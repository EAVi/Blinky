#include "SocketAddress.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#include <cstdio>

SocketAddress::SocketAddress()
{
}

SocketAddress::SocketAddress(uint16_t family)
{
	switch (family)
	{
		case AF_INET:
		{
			auto ipv4 = AsIPV4();
			memset(ipv4, 0, sizeof(sockaddr_in));
			ipv4->sin_port = 0;
			ipv4->sin_family = family;
			ipv4->sin_addr.S_un.S_addr = INADDR_ANY;
		}
		break;
		case AF_INET6:
		{
			auto ipv6 = AsIPV6();
			memset(ipv6, 0, sizeof(sockaddr_in6));
			ipv6->sin6_port = 0;
			ipv6->sin6_family = family;
			ipv6->sin6_flowinfo = 0;
			memcpy(&ipv6->sin6_addr, &in6addr_any, sizeof(IN6_ADDR));
		}
		break;
	}
}

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
		m_SockAddr.ss_family = AF_INET6;
		if (result == -1)
			printf( "failed to create ipv6 socket, error %d\n", WSAGetLastError());
		return;
	}

	sockaddr_in* sockaddr4 = AsIPV4();
	result = InetPtonA(AF_INET, inStr, &sockaddr4->sin_addr);
	if (result) //ipv4
	{
		m_SockAddr.ss_family = AF_INET;
		if(result == -1)
			printf("failed to create ipv4 socket, error %d\n", WSAGetLastError());
		return;
	}
}

void SocketAddress::SetAddressPort(uint16_t port)
{
	switch (m_SockAddr.ss_family)
	{
		case AF_INET:
		{
			auto address = AsIPV4();
			address->sin_port = htons(port);
			break;
		}
		case AF_INET6:
		{
			auto address = AsIPV6();
			address->sin6_port = htons(port);
			break;
		}
		default:
			printf("Address Family Unspecified, error %d\n", m_SockAddr.ss_family);
			break;
	}
}

const int SocketAddress::Size() const
{
	switch (m_SockAddr.ss_family)
	{
		case AF_INET: return sizeof(sockaddr_in);
		case AF_INET6: return sizeof(sockaddr_in6);
	}

	printf("unsupported or undefined address family, if family is not expected, use sizeof(sockaddr_storage)%d\n", m_SockAddr.ss_family);
	return sizeof(sockaddr_storage);
}
