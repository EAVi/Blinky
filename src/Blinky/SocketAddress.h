#ifndef BLINKY_SOCKET_ADDRESS_H
#define BLINKY_SOCKET_ADDRESS_H

//TODO more platforms
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdint>


//c++ style ipv4/6 socketaddress
class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort); //ipv4
	SocketAddress(const sockaddr& inSockAddr); //generalized
	SocketAddress(const char* inStr); //create socket address from string

private:
	FORCEINLINE sockaddr_in* AsIPV4();
	FORCEINLINE sockaddr_in6* AsIPV6();
	sockaddr_storage m_SockAddr;
};

#endif  //!BLINKY_SOCKET_ADDRESS_H
