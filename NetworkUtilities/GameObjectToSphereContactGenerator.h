#pragma once

#include "IContactGenerator.h"
#include "CollisionObjects.h"

class GameObjectToSphereContactGenerator : public IContactGenerator
{
public:
	GameObjectToSphereContactGenerator(SphereCollider* collider, const glm::vec3& position, float bounciness);
	~GameObjectToSphereContactGenerator() = default;

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const override;

private:
	SphereCollider* collider;
	glm::vec3 position;
	float bounciness;
};