#ifndef __MSTACK__
#define __MSTACK__

#include <sys/mman.h>
#include <iostream>
#include <exception>

void mcpy(void* dest, void* src, int size);

template<typename T>
struct mstack
{
	static const int EXPAND_FACTOR = 2;
	static const int DEFAULT_CAPACITY = 32;

	size_t capacity;
	size_t size;

	T* data;

	void set_capacity(size_t cap)
	{
		void* mem = mmap(nullptr, cap*sizeof(T), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (mem)
		{
			mcpy((T*)mem, data, (cap < capacity ? cap : capacity)*sizeof(T));
			munmap(data, capacity*sizeof(T));
			data = (T*)mem;
			capacity = cap;
		}
	}

	void expand()
	{
		size_t new_cap = capacity * EXPAND_FACTOR;
		void* mem = mmap(nullptr, new_cap*sizeof(T), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (mem)
		{
			mcpy((T*)mem, data, capacity*sizeof(T));
			munmap(data, capacity*sizeof(T));
			data = (T*)mem;
			capacity = new_cap;
		}
	}

	void push(const T& item)
	{
		if (size == capacity)
			expand();
		data[size++] = item;
	}

	T& push()
	{
		if (size == capacity)
			expand();
		return data[size++];
	}

	T& pop()
	{
		if (size > 0)
			return data[size-- - 1];
		std::abort();
	}

	T& peek()
	{
		if (size > 0)
			return data[size - 1];
		std::abort();
	}

	T peek_value()
	{
		return peek();
	}

	void remove(int pos)
	{
		if (pos < 0 || pos >= size)
			std::abort();
		swap(data[pos], data[size - 1]);
		pop();
	}

	T& operator[] (size_t index)
	{
		if (index >= 0 && index < size)
			return data[index];
		std::abort();
	}

	void init(size_t capacity = DEFAULT_CAPACITY)
	{
		size = 0;
		if (data)
			munmap(data,capacity*sizeof(T));
		this->capacity = capacity;
		void* mem = mmap(nullptr, capacity*sizeof(T), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (mem)
			data = (T*)mem;
	}

	mstack(size_t capacity = DEFAULT_CAPACITY) :capacity(capacity)
	{
		void* mem = mmap(nullptr, capacity*sizeof(T), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (mem)
			data = (T*)mem;
	}

	~mstack()
	{
		munmap(data, capacity*sizeof(T));
	}
};

#endif