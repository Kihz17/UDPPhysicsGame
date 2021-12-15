#pragma once

#include "ClientGameObject.h"
#include "CollisionObjects.h"

class SphereGameObject : public ClientGameObject
{
public:
	SphereGameObject(int id, const glm::mat4& transform, float bounciness, float mass, float radius)
		: ClientGameObject(id, transform, bounciness, mass), collider(new SphereCollider())
	{
		collider->radius = radius;
	}

	virtual ~SphereGameObject() { delete collider; }

	inline virtual GameObjectType GetType() const override { return GameObjectType::Sphere; }

	inline virtual void* GetColliderObject() override { return collider; }
	inline virtual CollisionHandlerType GetCollisionHandlerType() const override { return CollisionHandlerType::Sphere; }

private:
	SphereCollider* collider;
};