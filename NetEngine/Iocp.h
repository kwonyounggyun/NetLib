#pragma once
#include <vector>

class IOCP
{
	friend DWORD __stdcall ThreadCallback(VOID* param);
private:
	HANDLE h_cp_;
	std::vector<HANDLE> vec_threads_;
	
	HANDLE h_start_event_;
	HANDLE h_end_event_;

private:
	VOID ThreadCallback(VOID* param);

public:
	IOCP();
	virtual ~IOCP();

	BOOL Begin();
	BOOL End();
	BOOL RegisterSocketToIOCP(SOCKET socket, DWORD completionkey);

	BOOL OnIoRead(VOID* object, DWORD number_of_byte_transpered);
	BOOL OnIoWrote(VOID* object, DWORD number_of_byte_transpered);
};
