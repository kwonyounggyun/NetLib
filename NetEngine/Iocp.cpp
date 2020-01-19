#include "pch.h"
#include "iocp.h"
#include <iostream>
#include "NetMessage.h"
#include "Session.h"
#include "TCPSession.h"
#include "UDPSession.h"

DWORD __stdcall ThreadCallback(VOID* param)
{
	IOCP* iocp = reinterpret_cast<IOCP*>(param);
	SetEvent(iocp->h_start_event_);
	iocp->RunThread();

	return 0;
}

VOID IOCP::RunThread()
{
	DWORD transferred_bytes;
	DWORD completion_key;
	OVERLAPPED_EX* overlapped;
	while (1)
	{
		BOOL result = GetQueuedCompletionStatus(h_cp_, &transferred_bytes, &completion_key, (LPOVERLAPPED*) &overlapped, INFINITE);

		Session* session = reinterpret_cast<Session*>(overlapped->object);
		if (!result || (result && !transferred_bytes))
		{
			if (overlapped->type == IO_TYPE::IO_ACCEPT)
				OnConnect(session);
			else
				OnDisconnect(session);

			continue;
		}

		
		switch (overlapped->type)
		{
		case IO_TYPE::IO_READ:
			OnRead(session);
			break;
		case IO_TYPE::IO_WRITE:
			OnWrite(session);
			break;
		default:
			OnDisconnect(session);
			break;
		}
	}

	return VOID();
}

BOOL IOCP::OnConnect(Session* session)
{
	return 0;
}

BOOL IOCP::OnDisconnect(Session* session)
{
	return 0;
}

BOOL IOCP::OnWrite(Session* session)
{
	//session->WirteComplete();

	return 0;
}

BOOL IOCP::OnRead(Session* session)
{
	//session->Read();

	return 0;
}

IOCP::IOCP():h_cp_(nullptr), h_start_event_(nullptr), h_end_event_(nullptr)
{
}

IOCP::~IOCP()
{
}

BOOL IOCP::Begin(DWORD apply_thread_count)
{
	if (h_cp_ != NULL)
		return FALSE;

	if (apply_thread_count < 0)
		return FALSE;

	h_cp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, apply_thread_count);

	DWORD total_thread_count = apply_thread_count * 2;
	if (apply_thread_count == 0)
	{
		SYSTEM_INFO sys_info;
		GetSystemInfo(&sys_info);
		total_thread_count = sys_info.dwNumberOfProcessors * 2;
	}

	h_start_event_ = CreateEvent(NULL, FALSE, FALSE, NULL); //리셋안함
	if (h_start_event_ == NULL)
		return FALSE;

	for (int i = 0; i < apply_thread_count; i++)
	{
		HANDLE Thread = CreateThread(NULL, 0, ::ThreadCallback, this, 0, 0);
		vec_threads_.push_back(Thread);
		WaitForSingleObject(h_start_event_, INFINITE);
	}

	return TRUE;
}

BOOL IOCP::End()
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

BOOL IOCP::RegisterHandleToIOCP(HANDLE handle, DWORD completionkey)
{
	if (!handle || !completionkey)
		return FALSE;

	h_cp_ = CreateIoCompletionPort(handle, h_cp_, completionkey, 0);

	if (!h_cp_)
		return FALSE;

	return TRUE;
}
