#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public CMemoryPool<Session, 1000>, public CMultiThreadSync<Session> //이거 인터페이스로 분리해야함
{
private:
	SOCKET socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF]; //받는 역할만 하는 버퍼
	DWORD recv_byte_;
	DWORD recv_flag_;

	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;

public:
	Session();
	~Session();

	const SOCKET& Socket() 
	{
		return socket_;
	}

	BOOL InitializeIOCP();
	
	BOOL Read(NetMessage& msg, DWORD number_of_byte); //메세지를 넘겨서 세션버퍼의 내용을 메세지 안에잇는 버퍼로 복사
	BOOL Write(NetMessage& msg, DWORD number_of_byte); //메세지를 넘겨서 메세지 버퍼안의 내용을 전송하게하자

	BOOL Accept(SOCKET listen_sock);
};

