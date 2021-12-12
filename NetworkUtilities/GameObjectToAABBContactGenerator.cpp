#include "GameObjectToAABBContactGenerator.h"
#include "CollisionHandler.h"
#include <iostream>
GameObjectToAABBContactGenerator::GameObjectToAABBContactGenerator(AABBCollider* collider, const glm::mat4& transform, float bounciness)
	: collider(collider),
	transform(transform),
	bounciness(bounciness)
{

}

size_t GameObjectToAABBContactGenerator::AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const
{
	size_t contactsGenerated = 0;
	for (GameObject* gameObject : gameObjects)
	{
		CollisionHandlerType collisionType = gameObject->GetCollisionHandlerType();
		const glm::mat4& transform = gameObject->GetTransform();
		float restitution = gameObject->bounciness + this->bounciness;

		if (collisionType == CollisionHandlerType::Sphere)
		{
			CollisionHandler::HandleSphereToAABB(contactInstance, contactsGenerated,
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
			std::cout << "No collision handler for type " << (int) collisionType << " (GameObject -> AABB Generator)" << std::endl;
		}
	}

	return contactsGenerated;
}