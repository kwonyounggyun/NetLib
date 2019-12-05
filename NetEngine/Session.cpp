#include "pch.h"
#include "Session.h"

Session::Session(SOCKET* socket):socket_(socket), recv_byte_(0), recv_flag_(0)
{
	ZeroMemory(buf_, MAX_BUF);
	wsa_buf_.buf = buf_;
	wsa_buf_.len = MAX_BUF;
}

Session::~Session()
{
	if (socket_ != NULL)
		closesocket(*socket_);

	socket_ = NULL;
}

BOOL Session::InitializeIOCP()
{
	if (socket_ == NULL)
		return FALSE;

	ZeroMemory(buf_, MAX_BUF);
	wsa_buf_.buf = buf_;
	wsa_buf_.len = MAX_BUF;

	INT value = WSARecv(*socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &overlapped_.overlap, NULL);
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return 0;
}

BOOL Session::Read(NetMessage& msg)
{
	msg((BYTE*)buf_, recv_byte_);

	return 0;
}
