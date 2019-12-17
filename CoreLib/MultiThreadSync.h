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

class Lock
{
private:
	CriticalSection* cs;
	explicit Lock():cs(nullptr) {};
public:
	Lock(CriticalSection* critical) :cs(critical)
	{
		cs->Enter();
	}

	~Lock()
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