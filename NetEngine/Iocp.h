#pragma once
#include <vector>
#include <unordered_map>
#include "NetMessage.h"
#include <unordered_map>
#include "Session.h"

class IOCP
{
private:
	friend DWORD __stdcall ThreadCallback(VOID* param);

	std::unordered_map<SESSION_ID, Session*> connect_sessions;

private:
	IOCP& operator=(const IOCP&) {}  //������԰ź�
	IOCP(const IOCP&) {}   //��������� �ź�

	/* ������ ���� ����� ����, ������ ���� ������ �˷��� �ʿ����.*/
	VOID RunThread();

	BOOL OnConnect(Session* session);
	BOOL OnDisconnect(Session* session);

	/*������ �����ε� ����ڿ��� ������ ���� ������ ������ �ʿ����.*/
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

	/*���� �����Ϳ� ���� ó���̴�.*/
	virtual BOOL Task(NetMessage* msg) = 0;
};
