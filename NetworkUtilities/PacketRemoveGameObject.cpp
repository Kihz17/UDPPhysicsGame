#include "PacketRemoveGameObject.h"

PacketRemoveGameObject::PacketRemoveGameObject(int gameObjectIdz)
	: gameObjectId(gameObjectId)
{

}

PacketRemoveGameObject::PacketRemoveGameObject(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketRemoveGameObject::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(3); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
}

Buffer PacketRemoveGameObject::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(3); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	return buffer;
}

void PacketRemoveGameObject::Deserialize(Buffer& buffer)
{
	gameObjectId = buffer.ReadUInt32LE();
}