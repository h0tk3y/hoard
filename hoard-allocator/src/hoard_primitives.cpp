#include "hoard_primitives.h"
#include "unistd.h"

block_header* get_block_header(void* block_data)
{
	block_header* result = (block_header*)((char*)block_data - sizeof(block_header));
	if (result->signature == block_header::HEADER_SIGNATURE)
		return result;
	malloc_intercept::trace("bad block header: it has signature ", result->signature, "\n");
	return nullptr;
};

size_t round_up(size_t sz, size_t alignment)
{
	return (sz + alignment - 1) / alignment*alignment;
};

char* round_up(char* addr, size_t alignment)
{
	return (char*)(((size_t)addr + alignment - 1) / alignment * alignment);
};

int log2(size_t x)
{
	int result = 1;
	while (x >>= 1)
		result++;
	return result;
};