#include "UDPSocket.h"
#include <cstdio>

UDPSocket::UDPSocket()
{
	m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_Socket == INVALID_SOCKET)
	{
		printf("failed to create socket %d\n", WSAGetLastError());
	}
}

UDPSocket::~UDPSocket()
{
	closesocket(m_Socket);
}

bool UDPSocket::Bind(SocketAddress& addr)
{
	sockaddr* psock = addr.AsSockAddr();
	int err = bind(m_Socket, psock, addr.Size());
	if (err)
	{
		printf("error on bind, code %d\n", WSAGetLastError());
		return false;
	}

	//TODO REMOVE
	int len = addr.Size();
	getsockname(m_Socket, addr.AsSockAddr(), &len);
	printf("socket bound to port %d", addr.AsIPV4()->sin_port);

	return true;
}

bool UDPSocket::SendTo(const void* buf, size_t len, SocketAddress& to)
{
	const sockaddr* psock = to.AsSockAddr();
	const int flags = 0; //none of the flags are useful for UDP, and none are supported on UNIX
	int err = sendto(m_Socket, (const char*)buf, len, flags, psock, to.Size());
	if (err < 0)
	{
		printf("error on sendto, code %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

int UDPSocket::RecvFrom(void * dst, size_t len, SocketAddress& from)
{
	int fromlength = from.Size();
	const int flags = 0; //none of the flags are useful for UDP, and none are supported on UNIX
	int recieved = recvfrom(m_Socket, (char*)dst, len, flags, from.AsSockAddr(), &fromlength);

	if (from.Size() != fromlength || from.Size() == 0)
	{
		printf("sockaddr size mismatch, %d\n", WSAGetLastError());
	}

	if (recieved < 0)
	{
		printf("error on recvfrom, code %d\n", WSAGetLastError());
	}

	return recieved;
}
