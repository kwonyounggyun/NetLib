#pragma once
#include <Windows.h>
#include <WinSock2.h>

class CustomIocp
{
private:
	HANDLE h_cp;

public:
	CustomIocp();
	~CustomIocp();

	VOID Begin();
	VOID End();
};

