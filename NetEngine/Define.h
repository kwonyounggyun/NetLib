#pragma once
#include "pch.h"

#define MAX_BUF 4096

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

enum MSG_TYPE : BYTE
{
	LOGIN,
	LOGOUT
};