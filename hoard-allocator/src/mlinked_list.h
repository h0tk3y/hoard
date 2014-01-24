#include "mstack.h"
#include <unistd.h>

template <typename T_item>
struct mlinked_list
{
	template <typename T>
	struct node
	{
		int next_index;
		int prev_index;
		T value;

		node(T value, int prev, int next) : value(value), next_index(next), prev_index(prev) {};
	};

	template <typename T>
	struct miterator
	{
		mlinked_list<T>& list;
		int current_index;

		miterator(mlinked_list<T>& list, int index) : list(list), current_index(index) {};

		T& operator*() { return list.nodes[current_index].value; }

		miterator& operator++() {
			if (is_valid())
				current_index = list.nodes[current_index].next_index;
			return *this;
		}

		miterator& operator--() {
			if (is_valid())
				current_index = list.nodes[current_index].prev_index;
			return *this;
		}

		bool is_valid() { return current_index != -1; }

		T cut()
		{
			if (current_index != -1) {
				int new_index = list.nodes[current_index].next_index;
				T result = list.cut_at_index(current_index);
				current_index = new_index;
				return result;
			}
			else
				std::abort();
		}
	};

	mstack<node<T_item>> nodes;
	mstack<int> free_indices;

	int size;
	int capacity;

	int head_index;
	int tail_index;

	void init()
	{
		size = 0;
		head_index = tail_index = -1;
		
		nodes.init();
		nodes.size = nodes.capacity;
		capacity = nodes.capacity;

		free_indices.init();
		for (int i = 0; i < capacity; ++i)
		{
			free_indices.push(i);
		}
	}

	mlinked_list()
	{
		init();
	}

	void expand()
	{
		int old_cap = capacity;
		nodes.expand();
		nodes.size = nodes.capacity;
		capacity = nodes.capacity;
		for (old_cap; old_cap < capacity; old_cap++)
			free_indices.push(old_cap);
	}

	T_item cut_at_index(int index)
	{
		node<T_item>& n = nodes[index];
		if (n.next_index == -1)
			tail_index = n.prev_index;
		else
			nodes[n.next_index].prev_index = n.prev_index;
		if (n.prev_index == -1)
			head_index = n.next_index;
		else
			nodes[n.prev_index].next_index = n.next_index;
		free_indices.push(index);
		size--;
		return n.value;
	}

	//returns node index
	int push_back(const T_item& value)
	{
		if (size == capacity)
			expand();

		int index = free_indices.pop();
		if (head_index == -1)
			head_index = index;
		else
			nodes[tail_index].next_index = index;

		node<T_item>& n = nodes[index];
		n.prev_index = tail_index;
		tail_index = index;
		n.next_index = -1;
		n.value = value;
		++size;
		return index;
	}

	//returns node index
	int push_front(const T_item& value)
	{
		if (size == capacity)
			expand();
		int index = free_indices.pop();
		node<T_item>& n = nodes[index];
		if (tail_index == -1)
			tail_index = index;
		else
			nodes[head_index].prev_index = index;
		n.next_index = head_index;
		head_index = index;
		n.prev_index = -1;
		n.value = value;
		++size;
		return index;
	}

	T_item& peek_front()
	{
		if (size > 0)
			return nodes[head_index].value;
		std::abort();
	}

	T_item pop_front()
	{
		if (size > 0)
			return iterator_at_id(head_index).cut();
		std::abort();
	}

	T_item& peek_back()
	{
		if (size > 0)
			return nodes[tail_index].value;
		std::abort();
	}

	T_item pop_back()
	{
		if (size > 0)
			return iterator_at_id(tail_index).cut();
		std::abort();
	}

	void move_to_stack_top(int id)
	{
		if (size <= 1 || id == tail_index) return;
		iterator_at_id(id).cut();
		if (free_indices.pop() != id) std::abort();
		nodes[id].next_index = -1;
		nodes[id].prev_index = tail_index;
		size++;
		nodes[tail_index].next_index = id;
		tail_index = id;
	}

	miterator<T_item> begin()
	{
		return miterator<T_item>(*this, head_index);
	}

	miterator<T_item> end()
	{
		return miterator<T_item>(*this, tail_index);
	}

	miterator<T_item> iterator_at_id(int id)
	{
		return miterator<T_item>(*this, id);
	}
};