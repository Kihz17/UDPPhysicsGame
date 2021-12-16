#pragma once

#include "IContactGenerator.h"
#include "ColliderObjects.h"

class MeshContactGenerator : public IContactGenerator
{
public:
	MeshContactGenerator(MeshCollider* collider, const glm::mat4& transform, float bounciness);
	~MeshContactGenerator();

	virtual size_t AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const override;

private:
	MeshCollider* collider;
	glm::mat4 transform;
	float bounciness;
};