#pragma once
#include "Iocp.h"
#include "Session.h"
#include <unordered_map>

class SessionBaseServer : private IOCP
{
private:
	SOCKET listen_socket_;
public:
	std::unordered_map<DWORD, Session*> map_session;

private:
	VOID AllocSession();

public:
	SessionBaseServer();
	~SessionBaseServer();

	VOID Begin(DWORD listen_port);

	virtual VOID IOCallback();
};