#include "pch.h"
#include "SessionBaseServer.h"

VOID SessionBaseServer::AllocSession()
{
	for (DWORD i = 0; i < 1000; i++)
	{
		Session* session = new Session();
		session->Accept(listen_socket_);
		IOCP::RegisterHandleToIOCP((HANDLE)session->Socket(), (DWORD)session);
		map_session.insert(std::make_pair(session->ID(), session));
	}
}

SessionBaseServer::SessionBaseServer()
{
	
}

SessionBaseServer::~SessionBaseServer()
{
	End();
	for (auto iter : map_session)
	{
		delete iter.second;
		map_session.erase(iter.first);
	}
}

VOID SessionBaseServer::Begin(DWORD listen_port)
{
	IOCP::Begin();

	listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(listen_port);

	if (bind(listen_socket_, (SOCKADDR*)& addr, sizeof(addr)) < 0)
	{
		return;
	}

	if (listen(listen_socket_, 1000) < 0)
	{
		return;
	}

	AllocSession();
}

VOID SessionBaseServer::IOCallback()
{
	DWORD byte_transfer;
	VOID* session_addr;
	OVERLAPPED_EX* overlapped = nullptr;
	while (TRUE)
	{
		BOOL successed = GetQueuedCompletionStatus(h_cp_, &byte_transfer, (LPDWORD)&session_addr, (LPOVERLAPPED*) &overlapped, NULL);

		Session* session = reinterpret_cast<Session*>(session_addr);
		switch (overlapped->type)
		{
		case IO_TYPE::IO_ACCEPT:
		{
			if (!successed || (successed && !byte_transfer))
			{
				if (overlapped->type == IO_TYPE::IO_ACCEPT)
				{
					session->InitializeIOCP();
				}
				else       //에러이거나 오류로 판단해서 연결끊고 다시 대기 상태로 만든다.
				{
					session->End();
					session->Accept(listen_socket_);
				}

				continue;
			}
		}
			break;
		case IO_TYPE::IO_READ:
		{
			session->Read();
		}
			break;
		case IO_TYPE::IO_WRITE:
		{
			//메세지 큐에서 삭제하는 작업
		}
			break;
		}
		
	}
}
