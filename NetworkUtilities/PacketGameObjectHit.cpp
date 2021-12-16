#include "PacketGameObjectHit.h"

PacketGameObjectHit::PacketGameObjectHit(int gameObjectId, const glm::vec3& direction)
	: gameObjectId(gameObjectId), direction(direction)
{

}

PacketGameObjectHit::PacketGameObjectHit(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketGameObjectHit::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(7); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteFloat(direction.x);
	buffer.WriteFloat(direction.y);
	buffer.WriteFloat(direction.z);
}

Buffer PacketGameObjectHit::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(7); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteFloat(direction.x);
	buffer.WriteFloat(direction.y);
	buffer.WriteFloat(direction.z);
	return buffer;
}

void PacketGameObjectHit::Deserialize(Buffer& buffer)
{
	gameObjectId = buffer.ReadUInt32LE();
	direction.x = buffer.ReadFloat();
	direction.y = buffer.ReadFloat();
	direction.z = buffer.ReadFloat();
}