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

	// Session��(��) ���� ��ӵ�
	virtual BOOL WirteComplete() override;
	virtual BOOL Read(DWORD data_length) override;

	BOOL GetPacket(CHAR* buf, DWORD data_length);
};