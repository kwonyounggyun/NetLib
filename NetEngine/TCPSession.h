#pragma once
#include "Session.h"

class TCPSession : Session
{
private:
	struct MsgBuffer
	{
		DWORD length;
		BYTE buf[MAX_BUF];
	};

	BYTE m_tcp_buffer[MAX_BUF];
	NetMessage* remain_msg;
	USHORT remain_size;
	USHORT m_tcp_buf_write_pos;
	CCircularQueue<NetMessage*> m_write_msg_queue;

	// Session을(를) 통해 상속됨
	virtual BOOL WirteComplete() override;
	virtual BOOL Read(DWORD data_length) override;

	BOOL GetPacket(BYTE* buf, DWORD data_length, USHORT& remain_pos);
	BOOL Write(NetMessage* msg);
};