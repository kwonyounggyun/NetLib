#pragma once
#include "Session.h"
#include <queue>

class TCPSession : Session
{
private:
	struct MsgBuffer
	{
		DWORD length;
		BYTE buf[MAX_BUF];
	};

	std::queue<MsgBuffer*> m_write_msg_queue;
};