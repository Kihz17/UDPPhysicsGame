#pragma once

#include "Packet.h"

class PacketUpdatePosition : public Packet
{
public:
	PacketUpdatePosition(int gameObjectId, float x, float y, float z);
	PacketUpdatePosition(Buffer& buffer);
	virtual ~PacketUpdatePosition() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 2 + sizeof(float) * 3; };

	int gameObjectId;
	float x, y, z;
};