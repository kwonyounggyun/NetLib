#include "pch.h"
#include "Session.h"

Session::Session() : m_buf_index(0)
{
	ZeroMemory(m_read_buf, MAX_BUF);
	m_wsa_read.buf = m_read_buf[m_buf_index];
	m_wsa_read.len = MAX_BUF;

	accept_overlapped.object = this;
	accept_overlapped.type = IO_TYPE::IO_ACCEPT;

	read_overlapped.object = this;
	read_overlapped.type = IO_TYPE::IO_READ;

	write_overlapped.object = this;
	write_overlapped.type = IO_TYPE::IO_WRITE;
}

Session::~Session()
{
	if (m_socket != NULL)
		closesocket(m_socket);

	m_socket = NULL;
}

BOOL Session::InitializeIOCP()
{
	if (m_socket == NULL)
		return FALSE;

	DWORD recv_byte = 0;
	DWORD recv_flag = 0;
	ZeroMemory(m_read_buf, MAX_BUF);
	m_wsa_read.buf = m_read_buf[m_buf_index];
	m_wsa_read.len = MAX_BUF;

	INT return_value = WSARecv(m_socket, &m_wsa_read, 1, &recv_byte, &recv_flag, &read_overlapped.overlap, NULL);   // 미리 한번 호출해줘야한다.
	if (return_value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL Session::TcpBind()
{
	CriticalLock lock(&m_critical);

	if (m_socket)
		return FALSE;

	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (m_socket == INVALID_SOCKET)
		return FALSE;

	BOOL no_delay = TRUE;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&no_delay, sizeof(no_delay));

	return TRUE;
}

BOOL Session::Listen(USHORT port, INT back_log)
{
	CriticalLock lock(&m_critical);

	if (port <= 0 || back_log <= 0)
		return FALSE;

	if (!m_socket)
		return FALSE;

	SOCKADDR_IN listen_socket_info;
	listen_socket_info.sin_family = AF_INET;
	listen_socket_info.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listen_socket_info.sin_port = htons(port);

	if (bind(m_socket, (SOCKADDR*)&listen_socket_info, sizeof(listen_socket_info)) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}

	if (listen(m_socket, back_log) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}

	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;

	if (setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}
	return TRUE;
}

BOOL Session::Accept(SOCKET listen_socket)
{
	if (!listen_socket)
		return FALSE;

	if (m_socket)
		return FALSE;

	DWORD recv = 0;
	m_wsa_read.buf = m_read_buf[m_buf_index];
	m_wsa_read.len = MAX_BUF;

	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_socket == INVALID_SOCKET)
		return FALSE;

	BOOL no_delay = TRUE;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&no_delay, sizeof(no_delay));

	if (!AcceptEx(listen_socket, m_socket, m_wsa_read.buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &recv, &accept_overlapped.overlap))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL Session::Connect(LPSTR address, USHORT port)
{
	CriticalLock lock(&m_critical);

	if (!address || port <= 0)
		return FALSE;

	if (!m_socket)
		return FALSE;

	SOCKADDR_IN remote_address_info;
	remote_address_info.sin_family = AF_INET;
	remote_address_info.sin_addr.S_un.S_addr = inet_addr(address);
	remote_address_info.sin_port = htons(port);

	if (WSAConnect(m_socket, (SOCKADDR*)&remote_address_info, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Session::Write(BYTE* data, DWORD data_length)
{
	CriticalLock lock(&m_critical);

	if (!m_socket)
		return FALSE;

	if (!data || data_length <= 0)
		return FALSE;

	DWORD write_byte = 0;
	DWORD write_flag = 0;
	WSABUF wsa_buf;
	m_wsa_write.buf = (CHAR*) data;
	m_wsa_write.len = data_length;

	INT return_value = WSASend(m_socket, &m_wsa_write, 1, &write_byte, write_flag, &write_overlapped.overlap, NULL);

	if (return_value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
		return FALSE;
	}
	return TRUE;
}

BOOL Session::Begin()
{
	if (m_socket)
		return FALSE;

	ZeroMemory(m_read_buf, MAX_BUF);
	m_wsa_read.buf = m_read_buf[m_buf_index];
	m_wsa_read.len = MAX_BUF;

	m_wsa_write.buf = nullptr;
	m_wsa_write.len = 0;

	return TRUE;
}

BOOL Session::End()
{
	if (m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}

	ZeroMemory(m_read_buf, MAX_BUF);

	return TRUE;
}

/*읽기 버퍼로 부터 읽어오는 역할만 합니다.*/
BOOL Session::ReadForIOCP(BYTE* data, DWORD& data_length)
{
	if (!m_socket)
		return FALSE;

	if (!data || data_length <= 0)
		return FALSE;

	CopyMemory(data, m_read_buf, data_length);

	return TRUE;
}

BOOL Session::InitailizeUDP()
{
	if (m_socket)
		return FALSE;

	m_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (m_socket == INVALID_SOCKET)
		return FALSE;

	return 0;
}
