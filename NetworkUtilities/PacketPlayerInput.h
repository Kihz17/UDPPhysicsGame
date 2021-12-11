#pragma once

#include "Packet.h"
#include <glm/glm.hpp>

class PacketPlayerInput : public Packet
{
public:
	PacketPlayerInput(int gameObjectId, int requestId, const glm::vec3& position);
	PacketPlayerInput(Buffer& buffer);
	virtual ~PacketPlayerInput() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return sizeof(int) * 3 + sizeof(float) * 3; };

	int gameObjectId;
	int requestId;
	glm::vec3 position;
};