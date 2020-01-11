#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "MultiThreadSync.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session
{
protected:
	CriticalSection m_critical;

	SOCKET m_socket;
	WSABUF m_wsa_read, m_wsa_write;
	CHAR m_read_buf[MAX_BUF];
	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;

protected:
	Session();
	virtual ~Session();

	BOOL Begin();
	BOOL End();

	//TCP
	BOOL InitializeIOCP();
	BOOL TcpBind();
	BOOL Listen(USHORT port, INT back_log);
	BOOL Accept(SOCKET listen_socket);
	BOOL Connect(LPSTR address, USHORT port);
	BOOL Write(BYTE* data, DWORD data_length);
	BOOL ReadForIOCP(BYTE* data, DWORD& data_length);

	//UDP
	BOOL InitailizeUDP();
	BOOL InitailizeIOCPUDP();

	virtual BOOL WirteComplete() = 0;
	virtual BOOL Read(BYTE* data, DWORD data_length) = 0;
	virtual BOOL Write(BYTE* data, DWORD data_length) = 0;

	
	const SOCKET& Socket()
	{
		return m_socket;
	}
};

