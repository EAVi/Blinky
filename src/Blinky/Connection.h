#ifndef BLINKY_CONNECTION_H
#define BLINKY_CONNECTION_H
#include <cinttypes>
#include "Packet.h"
#include "UDPSocket.h"

enum ConnectionState
{
	k_Listening,
	k_Handshake,
	k_Connected,
	k_Disconnected,
};

class Connection
{
public:
	Packet MakeSendPacket(uint8_t* data, unsigned size, PacketDescription desc);
	void CheckRecvPacket(Packet p);
private:
	uint32_t IncrementSend();
	void UpdateRecieveIndex(uint32_t index);
	void UpdateRecvAck(uint32_t index, uint32_t flag);
	uint32_t m_RecvIndex;
	uint32_t m_SendIndex;
	uint32_t m_SendAckIndex;
	uint32_t m_RecieveAckFlags;
	uint32_t m_SentAckFlags;

	uint32_t m_PacketsSent;
	uint32_t m_PacketsLost;

};

#endif //!BLINKY_CONNECTION_H