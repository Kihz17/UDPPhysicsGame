#include "PacketUpdateGameObjectPositions.h"

PacketUpdateGameObjectPositions::PacketUpdateGameObjectPositions(std::vector<GameObjectMoveUpdate> data)
	: data(data), numberOfObjects(data.size())
{

}

PacketUpdateGameObjectPositions::PacketUpdateGameObjectPositions(Buffer& buffer)
{
	Deserialize(buffer);
}

void PacketUpdateGameObjectPositions::Serialize(Buffer& buffer) const
{
	buffer.WriteUInt32BE(4); // Packet type ID
	buffer.WriteUInt32BE(numberOfObjects);
	for (const GameObjectMoveUpdate& update : data)
	{
		buffer.WriteUInt32BE(update.gameObjectId);
		buffer.WriteUInt32BE(update.requestId);
		buffer.WriteFloat(update.position.x);
		buffer.WriteFloat(update.position.y);
		buffer.WriteFloat(update.position.z);
		buffer.WriteFloat(update.velocity.x);
		buffer.WriteFloat(update.velocity.y);
		buffer.WriteFloat(update.velocity.z);
	}
}

Buffer PacketUpdateGameObjectPositions::Serialize() const
{
	Buffer buffer(GetSize());
	buffer.WriteUInt32BE(4); // Packet type ID
	buffer.WriteUInt32BE(numberOfObjects);
	for (const GameObjectMoveUpdate& update : data)
	{
		buffer.WriteUInt32BE(update.gameObjectId);
		buffer.WriteUInt32BE(update.requestId);
		buffer.WriteFloat(update.position.x);
		buffer.WriteFloat(update.position.y);
		buffer.WriteFloat(update.position.z);
		buffer.WriteFloat(update.velocity.x);
		buffer.WriteFloat(update.velocity.y);
		buffer.WriteFloat(update.velocity.z);
	}
	return buffer;
}

void PacketUpdateGameObjectPositions::Deserialize(Buffer& buffer)
{
	numberOfObjects = buffer.ReadUInt32LE();
	for (int i = 0; i < numberOfObjects; i++)
	{
		GameObjectMoveUpdate update;
		update.gameObjectId = buffer.ReadUInt32LE();
		update.requestId = buffer.ReadUInt32LE();
		update.position.x = buffer.ReadFloat();
		update.position.y = buffer.ReadFloat();
		update.position.z = buffer.ReadFloat();
		update.velocity.x = buffer.ReadFloat();
		update.velocity.y = buffer.ReadFloat();
		update.velocity.z = buffer.ReadFloat();
		data.push_back(update);
	}
}