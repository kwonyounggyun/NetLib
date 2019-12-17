#include "pch.h"
#include "iocp.h"
#include <iostream>
#include "NetMessage.h"

DWORD __stdcall ThreadCallback(VOID* param)
{
	IOCP* iocp = reinterpret_cast<IOCP*>(param);
	SetEvent(iocp->h_start_event_);
	iocp->IOCallback();

	return 0;
}

//VOID IOCP::IOCallback()
//{
//	BOOL successed = FALSE;
//	DWORD number_of_byte_transfer = 0;
//	VOID* completionport = nullptr;
//	OVERLAPPED_EX* overlapped = nullptr;
//
//	while (TRUE)
//	{
//		SetEvent(h_start_event_);
//
//		successed = GetQueuedCompletionStatus(h_cp_, &number_of_byte_transfer, (LPDWORD)&completionport, (LPOVERLAPPED*)&overlapped, INFINITE);
//		
//		if (!completionport)
//			return;
//
//		Session* session = reinterpret_cast<Session*>(overlapped->object);
//
//
//		if (!successed || (successed && !number_of_byte_transfer))
//		{
//			if (overlapped->type == IO_TYPE::IO_ACCEPT)
//			{
//				session->InitializeIOCP();
//			}
//			else       //에러이거나 오류로 판단해서 연결끊고 다시 대기 상태로 만든다.
//			{
//				session->End();
//				wait_sessions->Push(session);
//			}
//
//			continue;
//		}
//
//		switch (overlapped->type)
//		{
//		case IO_TYPE::IO_READ:
//		{
//			//패킷이 두개가 붙어오는 경우도 잇다. 그래서 msg의 생성을 세션에서 하는 것이 맞는것같다.
//			NetMessage* msg = new NetMessage();
//			session->Read(*msg, number_of_byte_transfer);
//			Task(msg);
//
//			delete msg;
//		}
//			break;
//		case IO_TYPE::IO_WRITE:
//			break;
//		}
//	}
//
//	return VOID();
//}

IOCP::IOCP():h_cp_(nullptr), h_start_event_(nullptr), h_end_event_(nullptr)
{
}

IOCP::~IOCP()
{
}

BOOL IOCP::Begin(DWORD apply_thread_count = 0)
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

//check 이함수 실패하면 프로그램 종료를 해야하는건가?? h_cp_에 저장하는데 반환값이 널이되면 기존 핸들 잃어버리는거아닌가?
BOOL IOCP::RegisterHandleToIOCP(HANDLE handle, DWORD completionkey)
{
	if (!socket || !completionkey)
		return FALSE;

	h_cp_ = CreateIoCompletionPort(handle, h_cp_, completionkey, 0);

	if (!h_cp_)
		return FALSE;

	return TRUE;
}

//BOOL IOCP::OnIoRead(VOID* object, DWORD number_of_byte_transpered)
//{
//	NetMessage* msg = new NetMessage();
//	Session* session = reinterpret_cast<Session*>(object);
//	BOOL successed = session->Read(*msg, number_of_byte_transpered);
//
//	if (!successed)
//		return FALSE;
//
//	
//
//	return TRUE;
//}
//
//BOOL IOCP::OnIoWrote(VOID* object)
//{
//	if (!object)
//		return FALSE;
//
//	Session* session = reinterpret_cast<Session*>(object);
//	session->WirteComplete();
//
//	return TRUE;
//}
