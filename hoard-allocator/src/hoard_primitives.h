#ifndef __HOARD__
#define __HOARD__

#include "mstack.h"
#include "cpu_id.h"
#include "mlinked_list.h"
#include <pthread.h>

#include "tracing.h"

struct superblock;

static const size_t ALIGNMENT = 8;

using namespace std;

struct block_header
{
	static const size_t HEADER_SIGNATURE = 0xABCDEF01;

	void* base_addr;
	size_t total_size;
	size_t signature;
	size_t alignment;
	superblock* owner;
};

block_header* get_block_header(void* block_data);

size_t round_up(size_t sz, size_t alignment);
char* round_up(char* addr, size_t alignment);

int log2(size_t x);

struct heap;

struct superblock
{
	static const size_t SUPERBLOCK_SIZE = 65536;

	int index_in_heap;

	heap* owner;
	int size_class;
	mstack<void*> blocks;

	pthread_mutex_t mutex;

	void* data;

	bool is_empty()
	{
		return blocks.size == SUPERBLOCK_SIZE >> size_class;
	}

	bool has_free_blocks()
	{
		return blocks.size != 0;
	}

	void reset(size_t size_class)
	{
		blocks.init(superblock::SUPERBLOCK_SIZE >> size_class);
		size_t block_size = 1 << size_class;
		size_t offset = 0;
		for (int block = 0; offset + block_size <= SUPERBLOCK_SIZE; block++, offset += block_size)
			blocks.push((void*)((char*)data + offset));
	}

	//size_class is a power of 2, which will be block_header size + data_size of blocks
	void set_size_class(int size_class)
	{

		size_t block_size = 1 << size_class;
		size_t data_size = block_size - sizeof(block_header);
		if (block_size > SUPERBLOCK_SIZE >> 1 || data_size <= 0)
			std::abort();
		this->size_class = size_class;
		reset(size_class);
		
	}

	void* allocate()
	{
		return blocks.pop();
	}

	void free(block_header* header)
	{
		blocks.push(header->base_addr);
	}

	void init()
	{
		mutex = PTHREAD_MUTEX_INITIALIZER;
		data = mmap(NULL, SUPERBLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		blocks.init();
		set_size_class(size_class);
	}

	superblock(int size_class)
	{
		this->size_class = size_class;
		init();
	}

	~superblock()
	{
		munmap(data, SUPERBLOCK_SIZE);
	}
};

struct hoard;

struct heap
{
	pthread_mutex_t mutex;
	
	hoard* owner;
	size_t allocated;
	size_t used;
	int number;

	//should be power of 2
	static const int EMPTINESS_GROUPS = 8;

	mstack<mstack<mlinked_list<superblock*>>> emptiness_groups_by_size_class;

	void init()
	{
		mutex = PTHREAD_MUTEX_INITIALIZER;
		allocated = 0;
		used = 0;
		
		int n_size_classes = log2(superblock::SUPERBLOCK_SIZE);
		emptiness_groups_by_size_class.init(n_size_classes);
		emptiness_groups_by_size_class.size = n_size_classes;
		for (int i = 0; i < n_size_classes; ++i)
		{
			//EMPTINESS_GROUPS+2 means that there's one more emptiness group for empty superblocks
			//and one more for full superblocks
			emptiness_groups_by_size_class[i].init(EMPTINESS_GROUPS + 2);
			emptiness_groups_by_size_class[i].size = EMPTINESS_GROUPS + 2;
			for (int j = 0; j < EMPTINESS_GROUPS + 2; ++j)
				emptiness_groups_by_size_class[i][j].init();
		}
	}

	heap()
	{
		init();
	}
};

struct hoard
{
	size_t emptiness_threshold_step;

	int calculate_emtiness_group(size_t empty_bytes)
	{
		if (!empty_bytes) return 0;
		size_t threshold = emptiness_threshold_step;
		for (int i = 1; i < heap::EMPTINESS_GROUPS + 2; ++i, threshold += emptiness_threshold_step)
		{
			if (empty_bytes < threshold)
				return i;
		}
		return heap::EMPTINESS_GROUPS + 1;
	}

	heap global_heap;
	mstack<heap> per_processor_heaps;

	void* allocate_big_obj(size_t size, size_t alignment = ALIGNMENT) 
	{
		size_t requested_size = size + sizeof(block_header)+alignment-1;
		void* mem = mmap(nullptr, requested_size, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		char* data_ptr = round_up((char*)mem + sizeof(block_header), alignment);
		void* header_ptr = data_ptr - sizeof(block_header);
		block_header* header = (block_header*)header_ptr;
		header->alignment = alignment;
		header->total_size = requested_size;
		header->base_addr = mem;
		header->owner = nullptr;
		header->signature = block_header::HEADER_SIGNATURE;
		return data_ptr;
	};

	void* allocate(size_t size, size_t alignment = ALIGNMENT)
	{
		if (!already_initialized)
			init();
			
		if (!size) return nullptr;
		
		//calculate the desired block size class
		/*size_t data_offset = round_up(sizeof(block_header), ALIGNMENT);*/
		size_t requested_size = sizeof(block_header) + size + alignment - 1;
		int size_class = log2(requested_size);
		
		if (requested_size >= superblock::SUPERBLOCK_SIZE/2)
			return allocate_big_obj(size, alignment);
		
		//find the thread's heap
		int heap_id = ((unsigned int)pthread_self()) % per_processor_heaps.size;
		heap& h = per_processor_heaps[heap_id];
			
		
		auto& emptiness_groups = h.emptiness_groups_by_size_class[size_class];
		//find an emptiness group of maximum full superblocks which have free blocks
		//i == 0 -- absolutely full, i == 1..n -- exactly what we need
		int group = -1;
		for (size_t i = 1; i < emptiness_groups.size; ++i)
		if (emptiness_groups[i].size > 0) {
			group = i;
			break; 
		}
		//if no superblock found, check other size classes for fully empty superblocks
		if (group == -1)
		{
			int eg_empty = heap::EMPTINESS_GROUPS + 1;
			for (size_t i = 0; i < h.emptiness_groups_by_size_class.size; ++i)
			if (h.emptiness_groups_by_size_class[i][eg_empty].size > 0)
			{   //reset superblock's size class to which we need
				superblock* sb = h.emptiness_groups_by_size_class[i][eg_empty].pop_back();
				
				sb->set_size_class(size_class);
				sb->index_in_heap =
					h.emptiness_groups_by_size_class[size_class][eg_empty].push_back(sb);
				
				group = eg_empty;
				break;
			}
		}
		
		
		//if no superblock found, apply for superblock
		if (group == -1)
		{		
			group = add_superblock_to_heap(h, size_class);
		}

		superblock &sb = *emptiness_groups[group].peek_back();
		void* base_address = sb.blocks.pop();
		char* data_addr = round_up((char*)base_address + sizeof(block_header), alignment);
		void* header_ptr = data_addr - sizeof(block_header);
		block_header* header = (block_header*)header_ptr;
		header->alignment = alignment;
		header->base_addr = base_address;
		header->owner = emptiness_groups[group].peek_back();
		header->signature = block_header::HEADER_SIGNATURE;
		header->total_size = requested_size;

		
		//move superblock to another emptiness group if needed
		int new_emptiness_group = calculate_emtiness_group((1 << size_class) * sb.blocks.size);
		if (new_emptiness_group != group)
		{
			superblock *s = emptiness_groups[group].pop_back();
			
			s->index_in_heap = emptiness_groups[new_emptiness_group].push_back(s);
			
		}

		h.used += 1 << size_class;
		
		return data_addr;
	}

	//returns the emptiness group of the added superblock
	int add_superblock_to_heap(heap& h, int size_class)
	{
		
		mstack<mlinked_list<superblock*>> &global_heap_emptiness_groups = global_heap.emptiness_groups_by_size_class[size_class];
		int emptiness_group = -1;
		for (int i = global_heap_emptiness_groups.size - 1; i > 0; --i)
		{
			if (global_heap_emptiness_groups[i].size > 0)
			{
				emptiness_group = i;
				break;
			}
		}
		
		if (emptiness_group != -1)
		{
			superblock *sb = global_heap.emptiness_groups_by_size_class[size_class][emptiness_group].pop_back();
			
			
			
			global_heap.allocated -= superblock::SUPERBLOCK_SIZE;
			global_heap.used -= (1 << size_class) * ((superblock::SUPERBLOCK_SIZE >> size_class) - sb->blocks.size);
			sb->index_in_heap = h.emptiness_groups_by_size_class[size_class][emptiness_group].push_back(sb);
			sb->owner = &h;
			h.allocated += superblock::SUPERBLOCK_SIZE;
			h.used += (1 << size_class) * ((superblock::SUPERBLOCK_SIZE >> size_class) - sb->blocks.size);
			
			
			
		}
		
		if (emptiness_group == -1)
		{
			
			int last_eg = h.emptiness_groups_by_size_class[size_class].size - 1;
			superblock *s = make_superblock(size_class);
			
			s->index_in_heap = h.emptiness_groups_by_size_class[size_class][last_eg].push_back(s);
			s->owner = &h;
			
			h.allocated += superblock::SUPERBLOCK_SIZE;
			
			emptiness_group = last_eg;
		}
		return emptiness_group;
	}

	superblock* make_superblock(int size_class)
	{
		void* mem = mmap(NULL, sizeof(superblock), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (!mem)
			return nullptr;
		superblock* result = (superblock*)mem;
		result->size_class = size_class;
		result->init();
		return result;
	}

	static const int FREE_SUPERBLOCKS_LIMIT = 4;
	//empty fraction is 0.5

	void free_big_obj(block_header* header)
	{
		munmap(header->base_addr, header->total_size);
	}

	void free(void* data_ptr)
	{
		if (!data_ptr || !already_initialized)
			return;
		
		block_header* header = get_block_header(data_ptr);
		if (!header)
		{
			return;
		}
		if (!header->owner)
		{
			free_big_obj(header);
			return;
		}
			
		superblock &sb = *header->owner;
		heap &h = *sb.owner;

		
		

		int emptiness_group = calculate_emtiness_group((1 << sb.size_class) * sb.blocks.size);

		sb.free(header);
		sb.owner->used -= 1 << sb.size_class;

		//move-to-front heuristic
		mlinked_list<superblock*> &s = h.emptiness_groups_by_size_class[sb.size_class][emptiness_group];
		s.move_to_stack_top(sb.index_in_heap);

		//arranging emptiness groups
		int new_emptiness_group = calculate_emtiness_group((1 << sb.size_class) * sb.blocks.size);
		if (new_emptiness_group != emptiness_group)
		{
			superblock* sss = h.emptiness_groups_by_size_class[sb.size_class][emptiness_group].pop_back(); 
			int index = h.emptiness_groups_by_size_class[sb.size_class][new_emptiness_group].push_back(sss);
			sss ->index_in_heap= index;
		}
		//return superblock to gloabal_heap
		if (&h != &h.owner->global_heap &&
			h.used < h.allocated >> 1 && h.used < h.allocated - FREE_SUPERBLOCKS_LIMIT * superblock::SUPERBLOCK_SIZE)
			return_block_to_global_heap(h, &sb);
	}

	void return_block_to_global_heap(heap& from, superblock* current_sb)
	{
		if (&from == &from.owner->global_heap)
			return;
		
		size_t size_class = 0;
		int emptiness_group = heap::EMPTINESS_GROUPS + 1;
		
		superblock *sb_to_move = NULL;

		//check fully-empty superblocks first
		for (size_t i = 0; i < from.emptiness_groups_by_size_class.size; ++i)
		if (from.emptiness_groups_by_size_class[i][emptiness_group].size > 0)
		{
			sb_to_move = from.emptiness_groups_by_size_class[i][heap::EMPTINESS_GROUPS+1].pop_back();
			break;
		}
		
		//then check other emptiness groups, from empty to full, if no empty superblock found
		for (--emptiness_group; emptiness_group > 0 && !sb_to_move; --emptiness_group)
		for (size_class = 0; size_class < from.emptiness_groups_by_size_class.size; ++size_class)
		if (from.emptiness_groups_by_size_class[size_class][emptiness_group].size > 0)
		{
			sb_to_move = from.emptiness_groups_by_size_class[size_class][emptiness_group].pop_back();
			++emptiness_group;
			break;
		}
		
		if (sb_to_move)
		{
			from.allocated -= superblock::SUPERBLOCK_SIZE;
			from.used -= superblock::SUPERBLOCK_SIZE - sb_to_move->blocks.size * (1 << sb_to_move->size_class);
			from.owner->global_heap.allocated += superblock::SUPERBLOCK_SIZE;
			from.owner->global_heap.used += superblock::SUPERBLOCK_SIZE - sb_to_move->blocks.size * (1 << sb_to_move->size_class);
				
			sb_to_move->index_in_heap = global_heap.emptiness_groups_by_size_class[sb_to_move->size_class][emptiness_group].push_back(sb_to_move);
			sb_to_move->owner = &global_heap;
			
		}
		else //idk if it will ever be false
			std::abort();
	}


	size_t allocated()
	{
		size_t result = global_heap.allocated;
		for (size_t i = 0; i < per_processor_heaps.size; ++i)
			result += per_processor_heaps[i].allocated;
		return result;
	}

	size_t used()
	{
		size_t result = global_heap.used;
		for (size_t i = 0; i < per_processor_heaps.size; ++i)
			result += per_processor_heaps[i].used;
		return result;
	}
	
	pthread_mutex_t init_lock;
	
	bool already_initialized = false;
	pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	void init()
	{
		pthread_mutex_lock(&init_mutex);
		if (already_initialized)
			return;
		already_initialized = true;
		emptiness_threshold_step = superblock::SUPERBLOCK_SIZE / heap::EMPTINESS_GROUPS;
		global_heap.init();
		global_heap.owner = this;
		int cores = get_cores_count();
		per_processor_heaps.set_capacity(cores);
		per_processor_heaps.size = cores;
		for (int i = 0; i < cores; ++i)
		{
			per_processor_heaps[i].init();
			per_processor_heaps[i].owner = this;
			per_processor_heaps[i].allocated = 0;
			per_processor_heaps[i].used = 0;
		}
		pthread_mutex_unlock(&init_mutex);
	}

	hoard()
	{
	}
	
	~hoard()
	{
		already_initialized = false;
	}
	
	
};

#endif