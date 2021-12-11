#include "PacketPlayerInput.h"

PacketPlayerInput::PacketPlayerInput(int gameObjectId, int requestId, const glm::vec3& position)
	: gameObjectId(gameObjectId), requestId(requestId), position(position)
{

}

PacketPlayerInput::PacketPlayerInput(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketPlayerInput::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(0); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteUInt32BE(requestId);
	buffer.WriteFloat(position.x);
	buffer.WriteFloat(position.y);
	buffer.WriteFloat(position.z);
}

Buffer PacketPlayerInput::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(0); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteUInt32BE(requestId);
	buffer.WriteFloat(position.x);
	buffer.WriteFloat(position.y);
	buffer.WriteFloat(position.z);
	return buffer;
}

void PacketPlayerInput::Deserialize(Buffer& buffer)
{
	gameObjectId = buffer.ReadUInt32LE();
	requestId = buffer.ReadUInt32LE();
	position.x = buffer.ReadFloat();
	position.y = buffer.ReadFloat();
	position.z = buffer.ReadFloat();
}