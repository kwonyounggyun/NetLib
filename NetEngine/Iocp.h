#pragma once
#include <vector>
#include <unordered_map>

class IOCP
{
	friend DWORD __stdcall ThreadCallback(VOID* param);
private:
	IOCP& operator=(const IOCP&) {}  //복사대입거부
	IOCP(const IOCP&) {}   //복사생성자 거부

protected:
	HANDLE h_cp_;
	std::vector<HANDLE> vec_threads_;
	
	HANDLE h_start_event_;
	HANDLE h_end_event_;

protected:
	IOCP();
	virtual ~IOCP();

	BOOL Begin(DWORD thread_count = 0);
	BOOL End();
	BOOL RegisterHandleToIOCP(HANDLE handle, DWORD completionkey);

	/*BOOL OnIoRead(VOID* object, DWORD number_of_byte_transpered);
	BOOL OnIoWrote(VOID* object);*/

	virtual VOID IOCallback() = 0;
};
