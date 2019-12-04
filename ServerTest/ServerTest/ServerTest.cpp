#include "NetSession.h"
#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>

#pragma comment (lib, "ws2_lib")

#define MAX_BUF 4096

using namespace std;

HANDLE cp;

int main(void)
{
	WSAData wsa_data;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0) return;

	SOCKET server_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN sock_addr;
	sock_addr.sin_port = htons(9000);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if (bind(server_sock, (SOCKADDR*)& sock_addr, sizeof(SOCKADDR_IN)) != 0) return;

	listen(server_sock, 5); // 5개만

	cp = CreateIoCompletionPort((HANDLE)server_sock, NULL, (ULONG_PTR)&server_sock, 0);

	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	for (int i = 0; i < sys_info.dwNumberOfProcessors * 2; i++)
	{
		HANDLE thread = CreateThread(NULL, 0, ::ThreadCallBack, this, 0, NULL);
	}

	cout << "끝" << endl;

	WSACleanup();
	return 1;
}

DWORD __stdcall ThreadCallBack(VOID* param)
{
	DWORD read_byte = 0;
	DWORD completion_key = 0;
	OVERLAPPED* overlapped;
	OVERLAPPED_EX* overlapped_ex;
	BYTE buf[MAX_BUF];

	while (true)
	{
		GetQueuedCompletionStatus(cp, &read_byte, (PULONG_PTR)completion_key, &overlapped, INFINITE);

		if (!completion_key)
			return;

		overlapped_ex = (OVERLAPPED_EX*)overlapped;

		switch (overlapped_ex->io_type)
		{
		case IO_TYPE::ACCEPT:
		{
			SOCKET* server = reinterpret_cast<SOCKET*>(completion_key);

			Session* session = new Session();
			AcceptEx(*server, session->info.socket,
				buf,
				MAX_BUF - (sizeof(SOCKADDR_IN) + 16) * 2,
				sizeof(SOCKADDR_IN) + 16,
				sizeof(SOCKADDR_IN) + 16,
				&read_byte,
				(OVERLAPPED*)& session->overlapped);

			HANDLE h = CreateIoCompletionPort((HANDLE)session->info.socket, cp, (ULONG_PTR)session, 0);
			if (h == nullptr)
			{
				delete session;
			}
		}
			break;
		case IO_TYPE::READ:
		{

		}
			break;
		case IO_TYPE::WRITE:
		{

		}
			break;
		}
	}
}

VOID OnRead(SOCKET* s, WSABUF* buf)
{
	DWORD recv_byte;
	WSARecv(*s, buf, 1, &recv_byte, NULL, NULL, NULL); //다 읽고나서 할 루틴을 정의하는 부분이잇는데.. 객체로 만들면 이부분에다가 넣어야하겟는데

	wsasend
}