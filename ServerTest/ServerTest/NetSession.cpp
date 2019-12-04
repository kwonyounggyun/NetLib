#include "NetSession.h"

Session::Session() :packet_num(0), socket(NULL)
{

}

Session::~Session()
{
	closesocket(socket);
}

BOOL Session::Recv()
{	
	if (WSARecv(socket, buf, 1, &msg_size, &flag, (OVERLAPPED*)& overlapped, NULL) < 0) return FALSE;

	return TRUE;
}
