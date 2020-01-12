#pragma once
#include "pch.h"
#include "Define.h"
#include "MemoryPool.h"
#include <string>

class NetMessage
{
private:
	USHORT m_size;
	BYTE m_buf[MAX_BUF];
	USHORT m_readPos;
	USHORT m_writePos;
	
public:
	NetMessage() :m_size(sizeof(m_size)), m_readPos(0), m_writePos(sizeof(m_size))
	{
		ZeroMemory(m_buf, MAX_BUF);
	}

	NetMessage(VOID* buf, USHORT size) :m_size(size)
	{
		WriteByte(buf, size);
	}

	VOID WriteByte(VOID* data, DWORD len)
	{
		if ((MAX_BUF - m_writePos) < len)
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

	BOOL BufferClear()
	{
		m_size = sizeof(m_size);
		m_readPos = 0;
		m_writePos = sizeof(m_size);
		ZeroMemory(m_buf, MAX_BUF);
	}

	BYTE* GetBuffer()
	{
		CopyMemory(m_buf, &m_size, sizeof(m_size));
		return m_buf;
	}

	USHORT GetSize()
	{
		return m_size;
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