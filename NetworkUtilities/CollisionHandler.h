#pragma once

#include "ContactInstance.h"
#include "CollisionObjects.h"

class CollisionHandler
{
public:
	static bool HandleCollision(ContactInstance* contactInstance, GameObject* gameObject1, GameObject* gameObject2, size_t& contactsGenerated);

	static bool HandleSphereToSphere(ContactInstance* contactInstance, size_t& contactsGenerated,
		SphereCollider* sphereCollider1, const glm::vec3& position1,
		SphereCollider* sphereCollider2, const glm::vec3& position2,
		float restitution,
		GameObject* obj1, GameObject* obj2 = nullptr);

	static bool HandleSphereToAABB(ContactInstance* contactInstance, size_t& contactsGenerated,
		SphereCollider* sphereCollider, const glm::vec3& spherePosition, 
		AABBCollider* aabbCollider, const glm::mat4& aabbTransform,
		float restitution,
		GameObject* obj1, GameObject* obj2 = nullptr);

	static bool HandleSphereToMesh(ContactInstance* contactInstance, size_t& contactsGenerated,
		SphereCollider* sphereCollider, const glm::vec3& spherePosition,
		MeshCollider* meshCollider, const glm::mat4&  meshTransform,
		float restitution,
		GameObject* obj1, GameObject* obj2 = nullptr);

	//static bool HandleAABBToMesh(ContactInstance* contactInstance, GameObject* aabbObject, GameObject* meshObject);
	//static bool HandleAABBToAABB(ContactInstance* contactInstance, GameObject* aabbObject1, GameObject* aabbObject2);
	//static bool HandleMeshToMesh(ContactInstance* contactInstance, GameObject* meshObject1, GameObject* meshObject2);
};