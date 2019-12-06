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


class NetMessage:public CMemoryPool<NetMessage, 1000>
{
	friend class Session;
private:
	CHAR buf[MAX_BUF];
	DWORD size_;

public:

	VOID operator ()(BYTE* data, DWORD size)
	{
		if (size > (MAX_BUF- size_))
			return;

		CopyMemory(buf, data, size);
		size_ += size;
	}

	NetMessage();
	NetMessage(MSG_TYPE value):size_(0)
	{
		ZeroMemory(buf, MAX_BUF);
		CopyMemory(buf, &value, sizeof(value));
		size_ += sizeof(value);
	}

	VOID operator <<(DWORD value)
	{
		CopyMemory(buf+ size_, &value, sizeof(value));
		size_ += sizeof(value);
	}

	VOID operator >>(DWORD& value)
	{
		CopyMemory(&value, buf + size_, sizeof(value));
		size_ += sizeof(value);
	}
};