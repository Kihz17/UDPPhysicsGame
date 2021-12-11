#pragma once

#include "IContactGenerator.h"
#include "CollisionObjects.h"

class GameObjectToMeshContactGenerator : public IContactGenerator
{
public:
	GameObjectToMeshContactGenerator(MeshCollider* collider, const glm::mat4& transform, float bounciness);
	~GameObjectToMeshContactGenerator() = default;

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const override;

private:
	MeshCollider* collider;
	glm::mat4 transform;
	float bounciness;
};