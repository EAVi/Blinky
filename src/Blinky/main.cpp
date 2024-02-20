#include <cstdio>
#include <WinSock2.h>
#include "SocketAddress.h"

#pragma comment(lib,"WS2_32")

int main(int argc, char* args[])
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2,2), &wsadata);
	if(result == -1)
	{
		WSAGetLastError();
		return 1;
	}

	SocketAddress addr("ABCD:1234::1234:5678");

	WSACleanup();
	return 0;
}