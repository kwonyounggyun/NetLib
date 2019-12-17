#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public CMultiThreadSync<Session> //�޸� Ǯ������ ���Ǵ� ����ڿ��� �ñ�°� �´�. �̰� �������� �������̽��� �����һ��̴�.
{
private:
	CriticalSection cs;

	DWORD id_;
	SESSION_TYPE type_;

	SOCKET socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF]; //�޴� ���Ҹ� �ϴ� ����
	DWORD recv_byte_;
	DWORD recv_flag_;

	CCircularQueue<NetMessage*> msg_queue;

	OVERLAPPED_EX accept_overlapped, read_overlapped, write_overlapped;
	

	static DWORD IDAllocate();
public:
	Session();
	Session(SESSION_TYPE type);
	~Session();

	const SOCKET& Socket() 
	{
		return socket_;
	}

	BOOL InitializeIOCP();
	
	BOOL Read(NetMessage& msg, DWORD number_of_byte); //�޼����� �Ѱܼ� ���ǹ����� ������ �޼��� �ȿ��մ� ���۷� ����
	BOOL Write(NetMessage& msg, DWORD number_of_byte); //�޼����� �Ѱܼ� �޼��� ���۾��� ������ �����ϰ�����

	BOOL Accept(SOCKET listen_sock);
	BOOL End();

	//�׳� ť���� ������� ����� ������ WSASend�� �۾����̸� PENDING �̿��� ������ ������� �۾��� �Ϸ�ȴ�.
	BOOL WirteComplete();
	DWORD ID() { return id_; }
};

