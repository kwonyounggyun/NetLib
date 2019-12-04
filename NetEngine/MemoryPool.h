#pragma once
#include "pch.h"
#include <vector>
#include <assert.h>

template<typename T, int ALLOC_BLOCK = 50>
class MemoryPool
{
private:
	static UCHAR* free_pointer_;

	static VOID AllocBlock()
	{
		free_pointer_ = new UCHAR[sizeof(T) * ALLOC_BLOCK];
		UCHAR** current = reinterpret_cast<UCHAR**>(free_pointer_);
		UCHAR* next = free_pointer_;
		for (INT i = 0; i < ALLOC_BLOCK - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<UCHAR**>(next);
		}
		*current = 0;
	}

public:
	static VOID* operator new(std::size_t alloc_length)
	{
		//assert(sizeof(T) == alloc_length);
		assert(sizeof(T) >= sizeof(UCHAR*));

		if (!free_pointer_)
			AllocBlock();

		UCHAR* return_ptr = free_pointer_;
		free_pointer_ = *reinterpret_cast<UCHAR**>(return_ptr);

		std::cout << "return : " << reinterpret_cast<UCHAR**>(return_ptr) << std::endl;
		return return_ptr;
	}

	static VOID operator delete(VOID* delete_pointer)
	{
		*reinterpret_cast<UCHAR**>(delete_pointer) = free_pointer_;
		free_pointer_ = static_cast<UCHAR*>(delete_pointer);
	}
};

template<typename T, int ALLOC_BLOCK>
UCHAR* MemoryPool<T, ALLOC_BLOCK>::free_pointer_ = nullptr;