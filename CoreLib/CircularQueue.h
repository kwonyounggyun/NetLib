#pragma once
#include "stdafx.h"
#include "MultiThreadSync.h"

#define MAX_QUEUE_LENGTH 1024

template <class T>
class CircularQueue
{
public:
	CircularQueue()
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;
	}
	~CircularQueue() {}

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

	BOOL End() { return TRUE; }

	BOOL Push(T data)
	{
		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == mQueueHead)
			return FALSE;

		CopyMemory(&mQueue[TempTail], &data, sizeof(T));

		mQueueTail = TempTail;
		return TRUE;
	}

	BOOL Pop(T& data)
	{
		if (mQueueHead == mQueueTail)
			return FALSE;

		DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(&data, &mQueue[TempHead], sizeof(T));
		mQueueHead = TempHead;
		return TRUE;
	}

	BOOL IsEmpty()
	{
		if (mQueueHead == mQueueTail) return TRUE;
		return FALSE;
	}
};

template <class T>
class ConcerrentCircularQueue:CircularQueue<T>
{
public:
	ConcerrentCircularQueue() {}
	~ConcerrentCircularQueue() {}

private:
	CriticalSection critical_section;

public:
	BOOL Begin(VOID)
	{
		CriticalLock lock(&critical_section);

		return CircularQueue<T>::Begin();
	}

	BOOL End() { return CircularQueue<T>::End(); }

	BOOL Push(T data)
	{
		CriticalLock lock(&critical_section);

		return CircularQueue<T>::Push(data);
	}

	BOOL Pop(T& data)
	{
		CriticalLock lock(&critical_section);

		return CircularQueue<T>::Pop(data);
	}

	BOOL IsEmpty()
	{
		return CircularQueue<T>::IsEmpty();
	}
};