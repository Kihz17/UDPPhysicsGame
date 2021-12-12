#include "GameObjectToMeshContactGenerator.h"
#include "CollisionHandler.h"
#include <iostream>

GameObjectToMeshContactGenerator::GameObjectToMeshContactGenerator(MeshCollider* collider, const glm::mat4& transform, float bounciness)
	: collider(collider),
	transform(transform),
	bounciness(bounciness)
{

}

size_t GameObjectToMeshContactGenerator::AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const
{
	size_t contactsGenerated = 0;
	for (GameObject* gameObject : gameObjects)
	{
		CollisionHandlerType collisionType = gameObject->GetCollisionHandlerType();
		const glm::mat4& transform = gameObject->GetTransform();
		float restitution = gameObject->bounciness + this->bounciness;

		if (collisionType == CollisionHandlerType::Sphere)
		{
			CollisionHandler::HandleSphereToMesh(contactInstance, contactsGenerated,
				(SphereCollider*)gameObject->GetColliderObject(), glm::vec3(transform[3]),
				this->collider, this->transform,
				restitution,
				gameObject);
		}
		else if (collisionType == CollisionHandlerType::Cuboid)
		{

		}
		else if (collisionType == CollisionHandlerType::Mesh)
		{

		}
		else
		{
			std::cout << "No collision handler for type " << (int)collisionType << " (GameObject -> Mesh Generator)" << std::endl;
		}
	}

	return contactsGenerated;
}