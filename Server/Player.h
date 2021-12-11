#pragma once

#include "PlayerNetworkInfo.h"

#include <GameObject.h>
#include <CollisionObjects.h>

class Player : public GameObject
{
public:
	Player(int id, unsigned short port, sockaddr_in from, const glm::mat4& transform);
	~Player();

	// Inherited from GameObject
	inline virtual GameObjectType GetType() const override { return GameObjectType::Player; }

	virtual void* GetColliderObject() override { return collider; }
	inline virtual CollisionHandlerType GetCollisionHandlerType() const override { return CollisionHandlerType::Sphere; }

	PlayerNetworkInfo networkInfo;

	SphereCollider* collider;
};