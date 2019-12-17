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

	INT value = WSARecv(socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &read_overlapped.overlap, NULL);   // �̸� �ѹ� ȣ��������Ѵ�.
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return 0;
}

BOOL Session::Read(NetMessage& msg, DWORD number_of_byte)
{
	if (recv_byte_ <= 0)
		return FALSE;

	msg((BYTE*)buf_, recv_byte_);

	recv_byte_ -= number_of_byte;
	recv_flag_ = 0;

	INT value = WSARecv(socket_, &wsa_buf_, 1, &recv_byte_, &recv_flag_, &read_overlapped.overlap, NULL);  //���簡 �Ϸ������ �� �̸� ȣ���س����Ѵ�.
	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	return TRUE;
}

//�����ϸ� msg�� �Ҹ� å���� IOCP���� �մ�. �����ϸ� msg�Ҹ��� ���ǿ��� �����ϸ� WriteComplete�� �ϸ� �����Ǹ� ť�� �����ȴ�. ���������� �����ϴ� ������
//WSASend�� ������� �۾��� �̷�� ���� �����̴�.
BOOL Session::Write(NetMessage& msg, DWORD number_of_byte) //�̰� recv���� ���� �ٸ� �Ű����� 
{
	DWORD write_byte = 0;
	DWORD write_flag = 0;

	WSABUF wsa_tmp;

	wsa_tmp.buf = msg.buf;
	wsa_tmp.len = msg.size_;

	INT value = WSASend(socket_, &wsa_tmp, 1, &write_byte, write_flag, (OVERLAPPED*)& write_overlapped, NULL);

	if (value == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return FALSE;
	}

	msg_queue.Push(&msg);

	return TRUE;
}

BOOL Session::Accept(SOCKET listen_sock)
{
	if (!listen_sock)
		return FALSE;

	if (socket_ != NULL)
		return FALSE;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socket_ == INVALID_SOCKET)
		return FALSE;

	BOOL no_delay = TRUE;
	setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)& no_delay, sizeof(no_delay));

	DWORD recv;
	//�̰� ������ �򰥷Ǵ�. acceptex�� �Ϸ� �뺸�� listen_sock���ٰ� �ϴ°Ŵ�...
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
	delete msg;

	return TRUE;
}
