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

	//this is garbage and will not be in final code
	// sample args: 
	// --client 127.0.0.1 42069
	// --server 42069
	//client sends a packet to address
	//server waits and recieves
	if ((argc == 3) && strcmp(args[1], "--server") == 0)
	{
		SocketAddress addr(AF_INET);
		uint16_t port = (uint16_t)atoi(args[2]);
		addr.SetAddressPort(port);
		UDPSocket socket;
		socket.Bind(addr);

		SocketAddress from;
		char buf[1024];
		while (socket.RecvFrom(buf, sizeof(buf), from) == 0)
		{
			Sleep(16);
		}
		printf("recieved a packet saying: %s\n", buf);
	}
	if ((argc == 4) && strcmp(args[1], "--client") == 0)
	{
		SocketAddress addr(AF_INET);
		UDPSocket socket;
		socket.Bind(addr);

		SocketAddress sendaddr(args[2]);
		uint16_t port = (uint16_t)atoi(args[3]);
		sendaddr.SetAddressPort(port);
		char message[] = "hello";
		socket.SendTo(message, sizeof(message), sendaddr);
		printf("packet sent to server\n");
	}

	WSACleanup();
	return 0;
}