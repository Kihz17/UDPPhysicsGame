#include "PacketSpawnGameObject.h"

PacketSpawnGameObject::PacketSpawnGameObject(int gameObjectId, GameObjectType type, float x, float y, float z)
	: gameObjectId(gameObjectId), type(type), x(x), y(y), z(z)
{

}

PacketSpawnGameObject::PacketSpawnGameObject(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketSpawnGameObject::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(2); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteUInt32BE((int) type);
	buffer.WriteFloat(x);
	buffer.WriteFloat(y);
	buffer.WriteFloat(z);
}

Buffer PacketSpawnGameObject::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(2); // Packet type ID
	buffer.WriteUInt32BE(gameObjectId);
	buffer.WriteUInt32BE((int)type);
	buffer.WriteFloat(x);
	buffer.WriteFloat(y);
	buffer.WriteFloat(z);
	return buffer;
}

void PacketSpawnGameObject::Deserialize(Buffer& buffer)
{
	gameObjectId = buffer.ReadUInt32LE();
	type = (GameObjectType) buffer.ReadUInt32LE();
	x = buffer.ReadFloat();
	y = buffer.ReadFloat();
	z = buffer.ReadFloat();
}