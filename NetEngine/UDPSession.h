#pragma once
#include "Session.h"

class UDPSession : Session
{
	// Session을(를) 통해 상속됨
	virtual BOOL WirteComplete() override;
	virtual BOOL Read(DWORD data_length) override;
};

