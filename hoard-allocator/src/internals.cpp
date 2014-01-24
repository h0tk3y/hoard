#include <iostream>
#include "internals.h"

static hoard h;

namespace malloc_intercept
{
	void* internal_alloc(size_t size, size_t alignment)
	{
		return h.allocate(size, alignment);
	}

	void internal_free(void* ptr)
	{
		h.free(ptr);
	}

	void* internal_realloc(void *ptr, size_t size)
	{
		if (ptr == NULL)
			return internal_alloc(size, DEFAULT_ALIGNMENT);
		if (size == 0)
			return nullptr;
		void* new_data = NULL;
		auto head = get_block_header(ptr);
		if (size>0)
			new_data = internal_alloc(size, head->alignment);
		if (!new_data) return nullptr;
		mcpy(new_data, ptr, min(size, head->total_size - sizeof(block_header) - head->alignment + 1));
		internal_free(ptr);
		return new_data;
	}

	bool is_power_of_2(size_t n)
	{
		return ((n != 0) && !(n & (n - 1)));
	}

	bool is_valid_alignment(size_t alignment)
	{
		if ((alignment % sizeof(void*)) != 0)
			return false;

		if (!is_power_of_2(alignment))
			return false;

		return true;
	}
}