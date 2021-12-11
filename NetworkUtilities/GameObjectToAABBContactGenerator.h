#pragma once

#include "IContactGenerator.h"
#include "CollisionObjects.h"

class GameObjectToAABBContactGenerator : public IContactGenerator
{
public:
	GameObjectToAABBContactGenerator(AABBCollider* collider, const glm::mat4& transform, float bounciness);
	~GameObjectToAABBContactGenerator() = default;

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const override;

private:
	AABBCollider* collider;
	glm::mat4 transform;
	float bounciness;
};