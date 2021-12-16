#pragma once

#include "Packet.h"
#include "GameObject.h"

class PacketGameObjectHit: public Packet
{
public:
	PacketGameObjectHit(int gameObjectId, const glm::vec3& direction);
	PacketGameObjectHit(Buffer& buffer);
	virtual ~PacketGameObjectHit() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 2 + sizeof(glm::vec3); };

	int gameObjectId;
	glm::vec3 direction;
};