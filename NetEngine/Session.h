#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public CMemoryPool<Session, 1000>, public CMultiThreadSync<Session> //�̰� �������̽��� �и��ؾ���
{
private:
	SOCKET socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF]; //�޴� ���Ҹ� �ϴ� ����
	DWORD recv_byte_;
	DWORD recv_flag_;

	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;

public:
	Session();
	~Session();

	const SOCKET& Socket() 
	{
		return socket_;
	}

	BOOL InitializeIOCP();
	
	BOOL Read(NetMessage& msg, DWORD number_of_byte); //�޼����� �Ѱܼ� ���ǹ����� ������ �޼��� �ȿ��մ� ���۷� ����
	BOOL Write(NetMessage& msg, DWORD number_of_byte); //�޼����� �Ѱܼ� �޼��� ���۾��� ������ �����ϰ�����

	BOOL Accept(SOCKET listen_sock);
};

