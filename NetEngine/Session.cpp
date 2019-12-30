#include "pch.h"
#include "Session.h"

DWORD Session::IDAllocate()
{
	static DWORD id = 0;
	return id++;
}

Session::Session()
{
}

Session::Session(SESSION_TYPE type): recv_byte_(0), recv_flag_(0)
{
	id_ = IDAllocate();
	type_ = type;
	ZeroMemory(buf_, MAX_BUF);
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

BOOL Session::End()
{
	if (socket_)
	{
		closesocket(socket_);
		socket_ = NULL;
	}

	ZeroMemory(buf_, MAX_BUF);

	return TRUE;
}

BOOL Session::WirteComplete()
{
	NetMessage* msg;
	msg_queue.Pop(msg);
	//delete msg;

	return TRUE;
}
