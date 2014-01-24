#include "mstack.h"

void mcpy(void* dest, void* src, int size)
{
	uint8_t *pdest = (uint8_t*)dest;
	uint8_t *psrc = (uint8_t*)src;

	int loops = (size / sizeof(uint32_t));
	for (int index = 0; index < loops; ++index)
	{
		*((uint32_t*)pdest) = *((uint32_t*)psrc);
		pdest += sizeof(uint32_t);
		psrc += sizeof(uint32_t);
	}

	loops = (size % sizeof(uint32_t));
	for (int index = 0; index < loops; ++index)
	{
		*pdest = *psrc;
		++pdest;
		++psrc;
	}
};