#pragma once

#include "PlayerNetworkInfo.h"

#include <GameObject.h>

class Player : public GameObject
{
public:
	Player(int id, unsigned short port, sockaddr_in from, const glm::mat4& transform);
	~Player();

	// Inherited from GameObject
	inline virtual GameObjectType GetType() const override { return GameObjectType::Player; }

	PlayerNetworkInfo networkInfo;
};