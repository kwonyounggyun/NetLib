#include "CustomIocp.h"

CustomIocp::CustomIocp()
{
}

CustomIocp::~CustomIocp()
{
}

VOID CustomIocp::Begin()
{
	h_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SOCKET server = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server, (SOCKADDR*)&addr_in, sizeof(SOCKADDR));

	return VOID();
}
