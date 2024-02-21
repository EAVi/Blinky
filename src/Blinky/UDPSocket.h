#ifndef BLINKY_UDP_SOCKET_H
#define BLINKY_UDP_SOCKET_H

#include "SocketAddress.h"

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();
	bool Bind(SocketAddress& addr);
	bool SendTo(const void* buf, size_t len, SocketAddress& to);
	int RecvFrom(void * dst, size_t len, SocketAddress& from);
private:
	SOCKET m_Socket;
	
};

#endif // !BLINKY_UDP_SOCKET_H