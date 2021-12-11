#pragma once

#include "Packet.h"
#include "GameObject.h"

class PacketRemoveGameObject : public Packet
{
public:
	PacketRemoveGameObject(int gameObjectId);
	PacketRemoveGameObject(Buffer& buffer);
	virtual ~PacketRemoveGameObject() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 2; };

	int gameObjectId;
};