#pragma once
#include "Session.h"

class UDPSession : Session
{
	// Session��(��) ���� ��ӵ�
	virtual BOOL WirteComplete() override;
	virtual BOOL Read(DWORD data_length) override;
};

