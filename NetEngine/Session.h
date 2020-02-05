#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "MultiThreadSync.h"
#include "Define.h"
#include "TaskManager.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif


class Session
{
private:
	CriticalSection m_critical;

	SOCKET m_socket;
	WSABUF m_wsa_read;
	CHAR m_read_buf[MAX_BUF];
	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;
	
	SESSION_ID id;

private:
	static SESSION_ID AllocateSessionID()
	{
		static SESSION_ID session_id = 0;
		return session_id++;
	}

protected:
	Session();
	virtual ~Session();

	//TCP
	BOOL InitializeIOCP();
	BOOL TcpBind();
	BOOL Listen(USHORT port, INT back_log);
	BOOL Accept(SOCKET listen_socket);
	BOOL Connect(LPWSTR address, USHORT port);
	BOOL WriteTCP(VOID* data, DWORD data_length);
	BOOL ReadTCP();
	BOOL ReadForIOCP(BYTE* data, DWORD data_length, DWORD recv_offset, DWORD total_recv_length);

	//UDP
	BOOL InitailizeUDP();
	BOOL InitailizeIOCPUDP();
	BOOL WriteUDP(BYTE* data, DWORD data_length, SOCKADDR* addr);
	BOOL ReadUDP(SOCKADDR* addr);

	virtual BOOL WirteComplete() = 0;
	virtual BOOL Read(DWORD data_length) = 0;

public:
	virtual BOOL Begin();
	virtual BOOL End();

	const SOCKET& Socket()
	{
		return m_socket;
	}

	const SESSION_ID ID()
	{
		return id;
	}
};

