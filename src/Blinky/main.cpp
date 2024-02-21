#include <cstdio>
#include <WinSock2.h>
#include "SocketAddress.h"
#include "UDPSocket.h"

#include <Windows.h>
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

	char message[] = "hello";

	SocketAddress addr("127.0.0.1");
	addr.SetAddressPort(0);
	
	UDPSocket socket;
	socket.Bind(addr);
	socket.SendTo(message, sizeof(message), addr);
	SocketAddress from;
	auto ipv4a = addr.AsIPV4();
	auto ipv4b = from.AsIPV4();
	char buf[1024];
	Sleep(2);
	socket.RecvFrom(buf, sizeof(buf), from);

	WSACleanup();
	return 0;
}