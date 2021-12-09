#include "PacketUpdatePosition.h"

PacketUpdatePosition::PacketUpdatePosition(int gameObjectId, float x, float y, float z)
	: gameObjectId(gameObjectId), x(x), y(y), z(z)
{

}

PacketUpdatePosition::PacketUpdatePosition(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketUpdatePosition::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(0); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteFloat(x);
	buffer.WriteFloat(y);
	buffer.WriteFloat(z);
}

Buffer PacketUpdatePosition::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(0); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteFloat(x);
	buffer.WriteFloat(y);
	buffer.WriteFloat(z);
	return buffer;
}

void PacketUpdatePosition::Deserialize(Buffer& buffer)
{
	gameObjectId = buffer.ReadUInt32LE();
	x = buffer.ReadFloat();
	y = buffer.ReadFloat();
	z = buffer.ReadFloat();
}