#include "pch.h"
#include "TCPSession.h"

BOOL TCPSession::Accept(SOCKET listen_sock)
{
	if (!listen_sock)
		return FALSE;

	if (socket_ != NULL)
		return FALSE;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socket_ == INVALID_SOCKET)
		return FALSE;

	BOOL no_delay = TRUE;
	setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)&no_delay, sizeof(no_delay));

	DWORD recv;
	//이거 때문에 헷갈렷다. acceptex는 완료 통보를 listen_sock에다가 하는거다...
	if (!AcceptEx(listen_sock, socket_, buf_, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &recv, &accept_overlapped.overlap))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL TCPSession::Read(DWORD number_of_byte)
{
	if (number_of_byte <= 0)
		return FALSE;

	recv_byte_ -= number_of_byte;
	recv_flag_ = 0;

	INT value = WSARecv(socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &read_overlapped.overlap, NULL);  //복사가 완료됫으면 또 미리 호출해놔야한다.
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL TCPSession::Write(BYTE* packet, DWORD length)
{
	MsgBuffer* msg_buffer = new MsgBuffer();
	
	CopyMemory(&msg_buffer->buf, packet, length);

	return TRUE;
}

BOOL TCPSession::WirteComplete()
{
	return 0;
}
