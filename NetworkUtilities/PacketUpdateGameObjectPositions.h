#pragma once

#include "Packet.h"
#include <glm/glm.hpp>
#include <vector>

struct GameObjectMoveUpdate
{
	int gameObjectId;
	int requestId;
	glm::vec3 position;
	glm::vec3 velocity;
};

class PacketUpdateGameObjectPositions : public Packet
{
public:
	PacketUpdateGameObjectPositions(std::vector<GameObjectMoveUpdate> data);
	PacketUpdateGameObjectPositions(Buffer& buffer);
	virtual ~PacketUpdateGameObjectPositions() = default;

	virtual void Serialize(Buffer& buffer) const override;
	virtual Buffer Serialize() const override;
	virtual void Deserialize(Buffer& buffer) override;

	inline virtual size_t GetSize() const override { return (sizeof(int) * 2) + (data.size() * sizeof(GameObjectMoveUpdate)); };

	int numberOfObjects;
	std::vector<GameObjectMoveUpdate> data;
};