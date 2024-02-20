#include "SocketAddress.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#include <cstdio>

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
	sockaddr_in* addr = AsIPV4();
	addr->sin_family = AF_INET;
	addr->sin_addr.S_un.S_addr = htonl(inAddress);
	addr->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
	memset(&m_SockAddr, 0, sizeof(m_SockAddr));
	memcpy(&m_SockAddr, &inSockAddr, sizeof(inSockAddr));
}

SocketAddress::SocketAddress(const char* inStr)
{
	//function only accepts address, not port
	memset(&m_SockAddr, 0, sizeof(m_SockAddr));

	//note InetPton expects wchar_t, PtonA takes char
	int result;
	sockaddr_in6* sockaddr6 = AsIPV6();
	result = InetPtonA(AF_INET6, inStr, &sockaddr6->sin6_addr);
	if (result)//ipv6
	{
		if (result == -1)
			printf( "failed to create ipv6 socket, error %d\n", WSAGetLastError());
		return;
	}

	sockaddr_in* sockaddr4 = AsIPV4();
	result = InetPtonA(AF_INET, inStr, &sockaddr4->sin_addr);
	if (result) //ipv4
	{
		if(result == -1)
			printf("failed to create ipv4 socket, error %d\n", WSAGetLastError());
		return;
	}
}

sockaddr_in* SocketAddress::AsIPV4()
{
	return (sockaddr_in*)&m_SockAddr;
}

sockaddr_in6* SocketAddress::AsIPV6()
{
	return (sockaddr_in6*)&m_SockAddr;
}
