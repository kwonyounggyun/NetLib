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

BOOL TCPSession::Read(DWORD data_length)
{
	if (data_length <= 0)
		return FALSE;

	DWORD recv_byte = 0;
	DWORD recv_flag = 0;

	//여기서 패킷 분리
	GetPacket(m_read_buf[m_buf_index], data_length);

	m_buf_index = (m_buf_index + 1) % MAX_BUF_INDEX;

	m_wsa_read.buf = m_read_buf[m_buf_index];;
	m_wsa_read.len = MAX_BUF;

	INT value = WSARecv(m_socket, &m_wsa_read, 1, &recv_byte, &recv_flag, &read_overlapped.overlap, NULL);
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL TCPSession::GetPacket(CHAR* buf, DWORD data_length)
{
	USHORT length = 0;
	DWORD index = 0;
	USHORT remain_length = data_length;
	USHORT length_size = sizeof(length);

	while (remain_length >= length_size) //메세지 분리하기위한 최소조건
	{
		CopyMemory(&length, buf->buf + index, sizeof(length_size));
		NetMessage* msg = new NetMessage();
		
		if (length <= 0 || length > MAX_BUF)
			break;

		if (remain_length < length)
			break;

		msg->WriteByte(buf->buf + index, length);
		//만들어진 메세지 작업 큐에 넣음

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
