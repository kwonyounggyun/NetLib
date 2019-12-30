#pragma once
#include "pch.h"
#include "Define.h"
#include "MemoryPool.h"
#include <string>

class NetMessage:CMemoryPool<NetMessage, 1000>
{
public:
	
	static const DWORD HEADER = sizeof(USHORT);
	static const DWORD MSG_BUF_SIZE = 4096 - HEADER;
private:
	USHORT m_size;
	BYTE m_buf[MSG_BUF_SIZE];
	USHORT m_readPos;
	USHORT m_writePos;

	VOID WriteByte(VOID* data, DWORD len)
	{
		if ((MSG_BUF_SIZE - m_writePos) < len)
			return;

		CopyMemory(m_buf + m_writePos, data, len);
		m_writePos += len;
	}

	VOID ReadByte(VOID* data, DWORD len)
	{
		if ((m_size - m_readPos) < len)
			return;

		CopyMemory(data, m_buf + m_readPos, len);
		m_readPos += len;
	}
	
public:
	explicit NetMessage() :m_size(0), m_readPos(0), m_writePos(0)
	{
		ZeroMemory(m_buf, MSG_BUF_SIZE);
	}

	NetMessage(VOID* buf, USHORT size) :m_size(size)
	{
		WriteByte(buf, size);
	}

	BOOL BufferClear()
	{
		m_size = 0;
		m_readPos = 0;
		m_writePos = 0;
		ZeroMemory(m_buf, MSG_BUF_SIZE);
	}

	template<typename T>
	NetMessage& operator >>(T& arg)
	{
		ReadByte(&arg, sizeof(T));
	}

	template<>
	NetMessage& operator >>(std::wstring& arg)
	{
		size_t size;
		ReadByte(&size, sizeof(size_t));
		arg.resize(size);
		ReadByte((VOID*)arg.c_str(), size);
	}

	template<typename T>
	NetMessage& operator <<(const T& arg)
	{
		WriteByte(&arg, sizeof(T));
	}

	template<>
	NetMessage& operator <<(const std::wstring& arg)
	{
		size_t len = arg.length();
		WriteByte(&len, sizeof(size_t));
		WriteByte((VOID*)arg.c_str(), len);
	}
};