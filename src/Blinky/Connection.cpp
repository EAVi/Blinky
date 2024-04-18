#include "Connection.h"
#include <cstdio>
Packet Connection::MakeSendPacket(uint8_t* data, unsigned size, PacketDescription desc)
{
	Packet packet;
	packet.Header.AppName = htonl(k_AppName);
	packet.Header.PacketIndex = htonl(m_SendIndex);
	packet.Header.AckBitField = htonl(m_RecieveAckFlags);
	packet.Header.Description = desc;
	IncrementSend();
	memset(packet.Header.padding, 0, sizeof(packet.Header.padding));
	memcpy(packet.Data, data, size);
	return packet;
}

void Connection::CheckRecvPacket(Packet p)
{
	unsigned appname = ntohl(p.Header.AppName);
	unsigned packetindex = ntohl(p.Header.PacketIndex);
	unsigned ackindex = ntohl(p.Header.AckIndex);
	unsigned ackbitfield = ntohl(p.Header.AckBitField);
	PacketDescription desc = (PacketDescription)p.Header.Description;

	if (appname != k_AppName)
		return;

	UpdateRecieveIndex(packetindex);
}

uint32_t Connection::IncrementSend()
{
	m_SentAckFlags = m_SentAckFlags << 1;
	m_SendIndex++;
	return m_SendIndex;
}

//update recieve ack flags based on the packetindex 
void Connection::UpdateRecieveIndex(uint32_t index)
{
	int diff = index - m_RecvIndex;

	if (index < m_RecvIndex)
	{
		if (diff >= 32)
		{
			printf("strange case, old packet detected");
		}
		m_RecieveAckFlags |= 1 << (diff);
	}
	else
	{
		for (int i = 0; i < diff; ++i)
		{
			if (~m_SentAckFlags & 0x80000000) //leftmost bit is zero
			{
				m_PacketsLost++;
			}
			m_RecieveAckFlags |= m_RecieveAckFlags << 1;
		}
		m_RecieveAckFlags |= 1 << (diff);
	}
}

//update sent acks based on based on ackindex and flags
void Connection::UpdateRecvAck(uint32_t index, uint32_t flag)
{
	int diff = index - m_SendAckIndex;
	m_SendAckIndex = m_SendAckIndex | (flag >> diff);

	for (int i = 0; i < diff; ++i)
	{
		if (~m_SentAckFlags & 0x80000000)
			m_PacketsLost++; //TODO queue up missed packets for tracking what was missed
		m_SentAckFlags << 1;
	}

	//
	m_SentAckFlags = flag;
	m_SendAckIndex = index;
}
