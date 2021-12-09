#pragma once

#include "GameObject.h"
#include "PlayerNetworkInfo.h"

class Player : public GameObject
{
public:
	Player(unsigned short port, sockaddr_in from, const glm::mat4& transform);
	~Player();

	// Inherited from GameObject
	inline virtual CollisionHandlerType GetCollisionHandlerType() const override { return CollisionHandlerType::Sphere; }

	PlayerNetworkInfo networkInfo;
};