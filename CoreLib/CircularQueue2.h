#pragma once
#include "MultiThreadSync.h"
#include "CircularQueue.h"

template <typename T>
class CCircularQueue2 : public CMultiThreadSync<CCircularQueue2<T>>
{
public:
	CCircularQueue2(VOID)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;
	}
	~CCircularQueue2(VOID) {}

private:
	T mQueue[MAX_QUEUE_LENGTH];
	DWORD mQueueHead;
	DWORD mQueueTail;

public:
	BOOL Begin(VOID)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;

		return TRUE;
	}

	BOOL End(VOID) { return TRUE; }

	BOOL Push(T data)
	{
		typename CThreadSync sync;

		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

		if (TempTail == mQueueHead)
			return FALSE;

		CopyMemeory(&mQueue[TempTail], &data, sizeof(T));

		mQueueTail = TempTail;

		return TRUE;
	}

	BOOL Pop(T& data)
	{
		typename CThreadSync sync;
		if (mQueueHead == mQueueTail)
			return FALSE;

		DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(data, mQueue[TempHead], sizeof(T));

		mQueueHead = TempHead;

		return TRUE;
	}

	BOOL IsEmpty(VOID)
	{
		typename CThreadSync sync;

		if (mQueueHead == mQueueTail) return TRUE;

		return FALSE;
	}
};