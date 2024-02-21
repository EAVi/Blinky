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
	SocketAddress();
	SocketAddress(uint32_t inAddress, uint16_t inPort); //ipv4
	SocketAddress(const sockaddr& inSockAddr); //ipv4
	SocketAddress(const char* inStr); //create socket address from string ipv4/6
	void SetAddressPort(uint16_t port); // after port binding, assign the port
	inline sockaddr_in* AsIPV4() { return (sockaddr_in*)&m_SockAddr; };
	inline sockaddr* AsSockAddr() { return (sockaddr*)&m_SockAddr; };
	inline sockaddr_in6* AsIPV6() { return (sockaddr_in6*)&m_SockAddr; };
	const int Size() const;
private:
	sockaddr_storage m_SockAddr;
};

#endif  //!BLINKY_SOCKET_ADDRESS_H
