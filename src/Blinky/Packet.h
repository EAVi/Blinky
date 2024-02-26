#ifndef BLINKY_PACKET
#define BLINKY_PACKET

#include <cstdint>

const unsigned k_AppName = 0x424E4B59; // "BNKY" ascii representation, change this to something else in your game

//reason for the packet or description of the content of the packet
enum PacketDescription : uint8_t
{
	k_RequestConnection,
	k_AcceptConnection,
	k_NormalPacket,//Add what you want
	k_KeepAlive,
	k_Disconnect, //a kick or a disconnect
	MAX
};

struct PacketHeader
{
	unsigned AppName;
	unsigned PacketIndex;
	unsigned IndexBitField;
	PacketDescription Description;
	//timestamp is not needed if we have a known fixed timestep
};

const size_t k_MaxPacketSize = 1024;
const size_t k_PacketDataSize = k_MaxPacketSize - sizeof(PacketHeader);


struct Packet
{
	PacketHeader Header;
	uint8_t Data[k_PacketDataSize];
};

#endif // !BLINKY_PACKET
