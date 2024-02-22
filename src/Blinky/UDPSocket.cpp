#include "UDPSocket.h"
#include <cstdio>

UDPSocket::UDPSocket()
{
	//TODO INET6 support
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

	//try to set to non-blocking
	DWORD nonBlocking = 1;
	err = ioctlsocket(m_Socket, FIONBIO, &nonBlocking);
	if (err)
	{
		printf("error on setting to nonblocking, code %d\n", WSAGetLastError());
		return false;
	}

	//nice info to have
	int len = addr.Size();
	getsockname(m_Socket, addr.AsSockAddr(), &len);
	printf("socket bound to port %d\n", ntohs(addr.AsIPV4()->sin_port));
	return true;
}

bool UDPSocket::SendTo(const void* buf, size_t len, SocketAddress& to)
{
	const sockaddr* psock = to.AsSockAddr();
	const int flags = 0; //none of the flags are useful for UDP, and none are supported on UNIX

	int err = sendto(m_Socket, (const char*)buf, 1024, flags, psock, to.Size());
	if (err < 0)
	{
		printf("error on sendto, code %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

int UDPSocket::RecvFrom(void * dst, size_t len, SocketAddress& from)
{
	int fromlength = sizeof(sockaddr_in); //expected family unknown till the packet arrives

	const int flags = 0; //none of the flags are useful for UDP, and none are supported on UNIX
	int recieved = recvfrom(m_Socket, (char*)dst, len, flags, from.AsSockAddr(), &fromlength);

	if (recieved < 0)
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK) //recvfrom on nonblocking 
		{
			return 0;
		}
		printf("error on recvfrom, code %d\n", WSAGetLastError());
	}

	if (from.Size() != fromlength || from.Size() == 0)
	{
		printf("sockaddr size mismatch, %d\n", WSAGetLastError());
	}


	return recieved;
}
