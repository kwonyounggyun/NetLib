#include "pch.h"
#include "SessionBaseServer.h"

SessionBaseServer::SessionBaseServer(DWORD iocp_apply_thread_count)
{
	Begin(iocp_apply_thread_count);
}

SessionBaseServer::~SessionBaseServer()
{
	End();
}

VOID SessionBaseServer::IOCallback()
{
	return VOID();
}
