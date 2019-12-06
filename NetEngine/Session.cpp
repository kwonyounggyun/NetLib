#include "pch.h"
#include "Session.h"

Session::Session(): recv_byte_(0), recv_flag_(0)
{	ZeroMemory(buf_, MAX_BUF);
	wsa_buf_.buf = buf_;
	wsa_buf_.len = MAX_BUF;

	accept_overlapped.object = this;
	accept_overlapped.type = IO_TYPE::IO_ACCEPT;

	read_overlapped.object = this;
	read_overlapped.type = IO_TYPE::IO_READ;

	write_overlapped.object = this;
	write_overlapped.type = IO_TYPE::IO_WRITE;
}

Session::~Session()
{
	if (socket_ != NULL)
		closesocket(socket_);

	socket_ = NULL;
}

BOOL Session::InitializeIOCP()
{
	if (socket_ == NULL)
		return FALSE;

	ZeroMemory(buf_, MAX_BUF);
	wsa_buf_.buf = buf_;
	wsa_buf_.len = MAX_BUF;

	INT value = WSARecv(socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &read_overlapped.overlap, NULL);   // 미리 한번 호출해줘야한다.
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return 0;
}

BOOL Session::Read(NetMessage& msg, DWORD number_of_byte)
{
	if (recv_byte_ <= 0 || recv_byte_ < number_of_byte)
		return FALSE;

	msg((BYTE*)buf_, recv_byte_);

	recv_byte_ -= number_of_byte;
	recv_flag_ = 0;

	INT value = WSARecv(socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &read_overlapped.overlap, NULL);  //복사가 완료됫으면 또 미리 호출해놔야한다.
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Session::Write(NetMessage& msg, DWORD number_of_byte) //이건 recv랑은 조금 다를 거같은데 
{
}

BOOL Session::Accept(SOCKET listen_sock)
{
	if (!listen_sock)
		return FALSE;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socket_ == INVALID_SOCKET)
		return FALSE;

	BOOL no_delay = TRUE;
	setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)& no_delay, sizeof(no_delay));

	//이거 때문에 헷갈렷다. acceptex는 완료 통보를 listen_sock에다가 하는거다...
	if (!AcceptEx(listen_sock, socket_, buf_, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL, &accept_overlapped.overlap))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			END();

			return FALSE;
		}
	}

	return TRUE;
}
