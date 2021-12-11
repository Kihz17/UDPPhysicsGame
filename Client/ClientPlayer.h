#pragma once

#include "ClientGameObject.h"

#include <CollisionObjects.h>

class ClientPlayer : public ClientGameObject
{
public:
	ClientPlayer(int id, const glm::mat4& transform);
	virtual ~ClientPlayer();

	inline virtual GameObjectType GetType() const override { return GameObjectType::Player; }

	inline virtual void* GetColliderObject() override { return sphereCollider; };
	inline virtual CollisionHandlerType GetCollisionHandlerType() const override { return CollisionHandlerType::Sphere; }

private:
	SphereCollider* sphereCollider;
};