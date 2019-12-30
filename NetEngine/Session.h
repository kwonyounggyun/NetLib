#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "Iocp.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class IOCP;

class Session
{
protected:
	friend class IOCP;

	CriticalSection cs;

	DWORD id_;
	SESSION_TYPE type_;

	SOCKET socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF]; //받는 역할만 하는 버퍼
	DWORD recv_byte_;
	DWORD recv_flag_;

	CCircularQueue<NetMessage*> msg_queue;

	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;
	
	static DWORD IDAllocate();
	virtual BOOL Read(DWORD number_of_byte) = 0; 
	virtual BOOL WirteComplete() = 0;
public:
	Session();
	Session(SESSION_TYPE type);
	~Session();

	const SOCKET& Socket() 
	{
		return socket_;
	}

	BOOL InitializeIOCP();
	
	virtual BOOL Write(NetMessage& msg, DWORD number_of_byte) =  0; //메세지를 넘겨서 메세지 버퍼안의 내용을 전송하게하자

	BOOL End();

	
	DWORD ID() { return id_; }
};

