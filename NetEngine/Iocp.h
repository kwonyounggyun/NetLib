#pragma once
#include <vector>
#include <unordered_map>
#include "NetMessage.h"
#include <queue>
#include "Session.h"

class IOCP
{
private:
	friend DWORD __stdcall ThreadCallback(VOID* param);

	static std::queue<NetMessage*>  Messages;

private:
	IOCP& operator=(const IOCP&) {}  //복사대입거부
	IOCP(const IOCP&) {}   //복사생성자 거부

	/* 스레드 동작 사용자 연결, 해제에 대한 동작은 알려줄 필요없다.*/
	VOID RunThread();

	BOOL OnConnect(Session* session);
	BOOL OnDisconnect(Session* session);

	/*데이터 쓰기인데 사용자에게 쓰기의 내부 동작을 보여줄 필요없다.*/
	BOOL OnWrite(Session* session);
	BOOL OnRead(Session* session);

protected:
	HANDLE h_cp_;
	std::vector<HANDLE> vec_threads_;
	
	HANDLE h_start_event_;
	HANDLE h_end_event_;

public:
	IOCP();
	virtual ~IOCP();

	BOOL Begin(DWORD thread_count = 0);
	BOOL End();
	BOOL RegisterHandleToIOCP(HANDLE handle, DWORD completionkey);

	/*읽은 데이터에 대한 처리이다.*/
	virtual BOOL Task(NetMessage* msg) = 0;
};
