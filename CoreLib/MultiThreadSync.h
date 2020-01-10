#pragma once
#include "stdafx.h"

class CriticalSection
{
public:
	CriticalSection(VOID)
	{
		InitializeCriticalSection(&mSync);
	}

	~CriticalSection(VOID)
	{
		DeleteCriticalSection(&mSync);
	}

	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&mSync);
	}

	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&mSync);
	}

private:
	CRITICAL_SECTION mSync;
};

class CriticalLock
{
private:
	CriticalSection* cs;
public:
	CriticalLock(CriticalSection* critical) :cs(critical)
	{
		cs->Enter();
	}

	~CriticalLock()
	{
		cs->Leave();
	}
};

//�ϳ��� ���ؽ��� ���� ��ü�� �����Ҷ� ����մϴ�.
template<class T>
class CMultiThreadSync
{
	friend class CThreadSync;
public:
	class CThreadSync
	{
	public:
		CThreadSync()
		{
			T::mSync.Enter();
		}

		~CThreadSync()
		{
			T::mSync.Leave();
		}
	};

private:
	static CriticalSection mSync;
};

template<class T>
CriticalSection CMultiThreadSync<T>::mSync;