#pragma once
#include "Iocp.h"

class SessionBaseServer : private IOCP
{
public:
	SessionBaseServer(DWORD iocp_apply_thread_count);
	~SessionBaseServer();
	
	virtual VOID IOCallback();
};

