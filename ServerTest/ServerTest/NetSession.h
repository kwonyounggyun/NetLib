#pragma once
#include "MemoryPool.h"
#include <vector>
#include <iostream>
#include <WinSock2.h>

#define MAX_BUF 4096

typedef enum _Message_Type : INT32
{
	TOOL,
	BROADCAST,
}MSG_TYPE;

typedef enum io_type{ACCEPT, READ, WRITE} IO_TYPE;

typedef struct overlapped_ex
{
	OVERLAPPED overlapped;
	IO_TYPE io_type;
}OVERLAPPED_EX;

class Session : public MemoryPool<Session, 1000>
{
public:
	SOCKET socket;
	DWORD packet_num;
	OVERLAPPED_EX overlapped;
	WSABUF buf;
	DWORD msg_size;
	DWORD flag;

	Session();
	virtual ~Session();
	
	BOOL Begin();
	BOOL Connect();
	BOOL Recv();
	BOOL Send();
};