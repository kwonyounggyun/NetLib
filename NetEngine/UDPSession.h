#pragma once
#include "Session.h"

class UDPSession : Session
{
private:
	// Session을(를) 통해 상속됨
	virtual BOOL Read(NetMessage& msg, DWORD number_of_byte) override;
	virtual BOOL WirteComplete() override;

public:
	virtual BOOL Write(NetMessage& msg, DWORD number_of_byte) override;
};

