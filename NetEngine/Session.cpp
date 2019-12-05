#include "pch.h"
#include "Session.h"

Session::Session(SOCKET* socket):socket_(socket)
{
	ZeroMemory(buf_, MAX_BUF);
	wsa_buf_.buf = buf_;
	wsa_buf_.len = MAX_BUF;
}

Session::~Session()
{
	if (socket_ != NULL)
		closesocket(*socket_);

	socket_ = NULL;
}

BOOL Session::InitializeIOCP()
{

	return 0;
}
