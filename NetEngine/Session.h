#pragma once
#include "MemoryPool.h"
#include "NetMessage.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

class Session : public CMultiThreadSync<Session> //메모리 풀에대한 정의는 사용자에게 맡기는게 맞다. 이건 어디까지나 인터페이스를 제공할뿐이다.
{
private:
	CriticalSection cs;

	DWORD id_;
	SESSION_TYPE type_;

	SOCKET socket_;
	WSABUF wsa_buf_;
	CHAR buf_[MAX_BUF]; //받는 역할만 하는 버퍼
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
	
	BOOL Read(NetMessage& msg, DWORD number_of_byte); //메세지를 넘겨서 세션버퍼의 내용을 메세지 안에잇는 버퍼로 복사
	BOOL Write(NetMessage& msg, DWORD number_of_byte); //메세지를 넘겨서 메세지 버퍼안의 내용을 전송하게하자

	BOOL Accept(SOCKET listen_sock);
	BOOL End();

	//그냥 큐에서 순서대로 지우는 이유는 WSASend는 작업중이면 PENDING 이여서 무조건 순서대로 작업이 완료된다.
	BOOL WirteComplete();
	DWORD ID() { return id_; }
};

