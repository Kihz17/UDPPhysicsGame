#include "GameObjectToSphereContactGenerator.h"
#include "CollisionHandler.h"
#include <iostream>

GameObjectToSphereContactGenerator::GameObjectToSphereContactGenerator(SphereCollider* collider, const glm::vec3& position, float bounciness)
	: collider(collider),
	position(position),
	bounciness(bounciness)
{

}

size_t GameObjectToSphereContactGenerator::AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const
{
	size_t contactsGenerated = 0;
	for (GameObject* gameObject : gameObjects)
	{
		CollisionHandlerType collisionType = gameObject->GetCollisionHandlerType();
		const glm::mat4& transform = gameObject->GetTransform();
		float restitution = gameObject->bounciness + this->bounciness;

		if (collisionType == CollisionHandlerType::Sphere)
		{
			CollisionHandler::HandleSphereToSphere(contactInstance, contactsGenerated,
				(SphereCollider*)gameObject->GetColliderObject(), glm::vec3(transform[3]),
				this->collider, this->position,
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
			std::cout << "No collision handler for type " << (int)collisionType << " (GameObject -> Sphere Generator)" << std::endl;
		}
	}

	return contactsGenerated;
}