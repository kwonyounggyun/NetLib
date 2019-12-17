#pragma once
#include "pch.h"

#define MAX_BUF 4096

typedef INT32 SESSION_ID;

enum class IO_TYPE {
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED overlap;
	VOID* object;
	IO_TYPE type;
}OVERLAPPED_EX;

enum MSG_TYPE : DWORD
{
	LOGIN,
	LOGOUT
};

enum class SESSION_TYPE :BYTE
{
	TCP,
	UDP
};