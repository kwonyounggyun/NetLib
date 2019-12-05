#pragma once
#include <vector>

class Iocp
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
	Iocp();
	virtual ~Iocp();

	BOOL Begin();
	BOOL End();
};
