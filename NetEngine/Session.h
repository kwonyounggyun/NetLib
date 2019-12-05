#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public CMemoryPool<Session, 1000>
{
private:
	SOCKET* socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF];
	DWORD recv_byte_;
	DWORD recv_flag_;

	OVERLAPPED_EX overlapped_;

public:
	Session(SOCKET* socket);
	~Session();

	BOOL InitializeIOCP();
	
	BOOL Read(NetMessage& msg);
	BOOL Recv();
};

