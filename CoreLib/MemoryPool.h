#pragma once
#include "stdafx.h"
#include "MultiThreadSync.h"
#include <assert.h>

template <typename T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CMultiThreadSync<T>
{
public:
	static VOID* operator new(size_t allocLength)
	{
		typename CMultiThreadSync<T>::CThreadSync sync;

		//assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		if(!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	static VOID operator delete(VOID* deletePointer)
	{
		typename CMultiThreadSync<T>::CThreadSync sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		mFreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID allocBlock()
	{
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		UCHAR** Current = reinterpret_cast<UCHAR **>(mFreePointer);
		UCHAR *Next = mFreePointer;

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = 0;
	}

private:
	static UCHAR *mFreePointer;

protected:
	~CMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;