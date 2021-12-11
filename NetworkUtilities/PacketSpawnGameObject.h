#pragma once

#include "Packet.h"
#include "GameObject.h"

class PacketSpawnGameObject : public Packet
{
public:
	PacketSpawnGameObject(int gameObjectId, GameObjectType type, float x, float y, float z);
	PacketSpawnGameObject(Buffer& buffer);
	virtual ~PacketSpawnGameObject() = default;

	virtual void Serialize(Buffer & buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer & buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 3 + sizeof(float) * 3; };

	int gameObjectId;
	GameObjectType type;
	float x, y, z;
};