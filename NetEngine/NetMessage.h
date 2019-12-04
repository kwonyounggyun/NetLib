#pragma once
#include "pch.h"
#include "Define.h"
#include "MemoryPool.h"

#define MAKE_IN_OUT_OPERATOR(TYPE)												\
	VOID operator <<(TYPE* value) {														\
	CopyMemory(buf+size, value, sizeof(TYPE));										\
	size += sizeof(TYPE);}																		\
	VOID operator >>(TYPE* value) {														\
	CopyMemory(value, buf, sizeof(TYPE));												\
	size += sizeof(TYPE);}

class NetMessage:public MemoryPool<NetMessage, 1000>
{
private:
	CHAR buf[MAX_BUF];
	INT32 size;

	NetMessage();
public:
	NetMessage(MSG_TYPE value):size(0)
	{
		ZeroMemory(buf, MAX_BUF);
		CopyMemory(buf, &value, sizeof(value));
		size += sizeof(value);
	}

	VOID operator <<(DWORD value)
	{
		CopyMemory(buf+size, &value, sizeof(value));
		size += sizeof(value);
	}

	VOID operator >>(DWORD& value)
	{
		CopyMemory(&value, buf + size, sizeof(value));
		size += sizeof(value);
	}
};