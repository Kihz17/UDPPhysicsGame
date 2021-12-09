#include "PacketPlayerConnectSuccess.h"

PacketPlayerConnectSuccess::PacketPlayerConnectSuccess(int playerId)
	: playerId(playerId)
{

}

PacketPlayerConnectSuccess::PacketPlayerConnectSuccess(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketPlayerConnectSuccess::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(1); // Packet type ID
	buffer.WriteUInt32BE(playerId);
}

Buffer PacketPlayerConnectSuccess::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(1); // Packet type ID
	buffer.WriteUInt32BE(playerId);
	return buffer;
}

void PacketPlayerConnectSuccess::Deserialize(Buffer& buffer)
{
	playerId = buffer.ReadUInt32LE();
}