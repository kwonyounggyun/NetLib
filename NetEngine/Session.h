#pragma once
#include "MemoryPool.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public MemoryPool<Session, 1000>
{
private:
	SOCKET* socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF];

public:
	Session(SOCKET* socket);
	~Session();

	BOOL Send();
	BOOL Recv();
};

