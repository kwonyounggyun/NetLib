#pragma once
#include "Session.h"

class UDPSession : Session
{
private:
	// Session��(��) ���� ��ӵ�
	virtual BOOL Read(NetMessage& msg, DWORD number_of_byte) override;
	virtual BOOL WirteComplete() override;

public:
	virtual BOOL Write(NetMessage& msg, DWORD number_of_byte) override;
};

