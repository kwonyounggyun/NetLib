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
	
	NetMessage() :m_size(sizeof(m_size)), m_readPos(sizeof(m_size)), m_writePos(sizeof(m_size))
	{
		ZeroMemory(m_buf, MAX_BUF);
	}

	NetMessage(VOID* buf, USHORT size) :m_size(size)
	{
		WriteByte(buf, size);
	}

	NetMessage(NetMessage&);

public:
	static NetMessage* NewMsg()
	{
		return new NetMessage();
	}

	VOID WriteByte(VOID* data, USHORT len)
	{
		if ((MAX_BUF - m_writePos) < len)
			return;

		CopyMemory(m_buf + m_writePos, data, len);
		m_writePos += len;
	}

	VOID ReadByte(VOID* data, USHORT len)
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
		return *this;
	}

	template<typename T>
	NetMessage& operator <<(const T& arg)
	{
		WriteByte(&arg, sizeof(T));
		return *this;
	}


};

template<>
NetMessage& NetMessage::operator << <std::wstring> (const std::wstring& arg)
{
	size_t len = arg.length()*sizeof(wchar_t);
	WriteByte(&len, sizeof(size_t));
	m_size += sizeof(size_t);
	WriteByte((VOID*)arg.c_str(), len);
	m_size += len;
	return *this;
}

template<>
NetMessage& NetMessage::operator >> <std::wstring> (std::wstring& arg)
{
	size_t size;
	ReadByte(&size, sizeof(size_t));
	arg.resize(size);
	ReadByte((VOID*)arg.c_str(), size);
	return *this;
}