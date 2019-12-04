#include "pch.h"
#include "Iocp.h"
#include <iostream>

DWORD __stdcall ThreadCallback(VOID* param)
{
	Iocp* iocp = reinterpret_cast<Iocp*>(param);
	SetEvent(iocp->h_start_event_);
	iocp->ThreadCallback(iocp->h_cp_);

	return 0;
}

VOID Iocp::ThreadCallback(VOID* param)
{
	int count = 5;
	while (count > 0)
	{
		std::cout << "test" << GetCurrentThreadId() << std::endl;
		Sleep(10000);
		count--;
	}

	return VOID();
}

BOOL Iocp::Begin()
{
	if (h_cp_ != NULL)
		return FALSE;

	h_cp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	INT thread_count = sys_info.dwNumberOfProcessors * 2;
	vec_threads_.reserve(thread_count);

	h_start_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (h_start_event_ == NULL)
		return FALSE;

	for (int i = 0; i < thread_count; i++)
	{
		HANDLE Thread = CreateThread(NULL, 0, ::ThreadCallback, this, 0, 0);
		vec_threads_.push_back(Thread);
		WaitForSingleObject(h_start_event_, INFINITE);
		Sleep(10);
	}

	return TRUE;
}

BOOL Iocp::End()
{
	for (size_t i = 0; i < vec_threads_.size(); i++)
	{
		PostQueuedCompletionStatus(h_cp_, 0, 0, NULL);
	}

	for (auto iter = vec_threads_.begin(); iter != vec_threads_.end(); iter++)
	{
		WaitForSingleObject(*iter, INFINITE);
		CloseHandle(*iter);
	}

	if (h_start_event_)
		CloseHandle(h_start_event_);

	if (h_cp_)
	{
		CloseHandle(h_cp_);
		h_cp_ = NULL;
	}

	vec_threads_.clear();

	return TRUE;
}
