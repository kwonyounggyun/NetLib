#pragma once
#include "MultiThreadSync.h"

template <typename T>
class CCircularQueue2 : public CMultiThreadSync<CCircularQueue2<T>>
{
public:
	CCircularQueue2(VOID)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;
	}
	~CircularQueue2(VOID) {}

private:
	T mQueue[MAX_QUEUE_LENGTH];
	DWORD mQueueHead;
	DWORD mQueueTail;

public:
	BOOL Begin(VOID)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQeueue = mQueueTail = 0;

		return TRUE;
	}

	BOOL End(VOID) { return TRUE; }

	BOOL PUSH(T data)
	{
		CThreadSync sync;

		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

		if (TempTail == mQueueHead)
			return FALSE;

		CopyMemeory(&mQueue[TempTail], &data, sizeof(T));

		mQueueTail = TempTail;

		return TRUE;
	}

	BOOL Pop(T& data)
	{
		CThreadSync sync;

		if (mQueueHead == mQeueTail)
			return FALSE;

		DWORD DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(data, mQueue[TempHead], sieof(T));

		mQueueHead = TempHead;

		return TRUE;
	}

	BOOL IsEmpty(VOID)
	{
		CThreadSync sync;

		if (mQueuehead == mQueueTail) return TRUE;

		return FALSE;
	}
};