#pragma once
#include "Session.h"

class TCPSession : Session
{
private:

protected:

public:

private:
	virtual BOOL Read(DWORD number_of_byte) override;
	virtual BOOL WirteComplete() override;

public:
	BOOL Accept(SOCKET listen_sock);

	// Session��(��) ���� ��ӵ�
	virtual BOOL Write(NetMessage& msg, DWORD number_of_byte) override;
};