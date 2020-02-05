#pragma once
#include <concurrent_unordered_map.h>
#include "Session.h"

class SessionManager
{
private:
	//concurrency::concurrent_unordered_map<SESSION_ID, Session*> session_map;

private:
	SessionManager() {}
	~SessionManager() {}

public:
	BOOL ConnectSession(Session* session);
	BOOL ConnectSession(SESSION_ID id);
	BOOL CloseSession(Session* session);
	BOOL CloseSession(SESSION_ID id);
	Session* FindSession(Session* session);
	Session* FindSession(SESSION_ID id);
	BOOL isEmptySession() {
		return true;//session_map.empty();
	}

	static SessionManager& Instance()
	{
		static SessionManager manager;
		return manager;
	}
};

