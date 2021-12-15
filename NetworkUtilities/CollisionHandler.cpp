#include "CollisionHandler.h"

bool CollisionHandler::HandleCollision(ContactInstance* contactInstance, GameObject* gameObject1, GameObject* gameObject2, size_t& contactsGenerated)
{
	CollisionHandlerType collisionType1 = gameObject1->GetCollisionHandlerType();
	CollisionHandlerType collisionType2 = gameObject2->GetCollisionHandlerType();

	float restitution = gameObject1->bounciness + gameObject2->bounciness;
	if (collisionType1 == CollisionHandlerType::Sphere && collisionType2 == CollisionHandlerType::Sphere)
	{
		return HandleSphereToSphere(contactInstance, contactsGenerated,
			(SphereCollider*)gameObject1->GetColliderObject(), glm::vec3(gameObject1->GetTransform()[3]), 
			(SphereCollider*)gameObject2->GetColliderObject(), glm::vec3(gameObject2->GetTransform()[3]),
			restitution,
			gameObject1, gameObject2);
	}
	else if (collisionType1 == CollisionHandlerType::Sphere && collisionType2 == CollisionHandlerType::Cuboid)
	{
		return HandleSphereToAABB(contactInstance, contactsGenerated,
			(SphereCollider*)gameObject1->GetColliderObject(), glm::vec3(gameObject1->GetTransform()[3]),
			(AABBCollider*)gameObject2->GetColliderObject(), gameObject2->GetTransform(),
			restitution,
			gameObject1, gameObject2);
	}
	else if (collisionType1 == CollisionHandlerType::Cuboid && collisionType2 == CollisionHandlerType::Sphere)
	{
		return HandleSphereToAABB(contactInstance, contactsGenerated,
			(SphereCollider*)gameObject2->GetColliderObject(), glm::vec3(gameObject2->GetTransform()[3]),
			(AABBCollider*)gameObject1->GetColliderObject(), gameObject1->GetTransform(),
			restitution,
			gameObject2, gameObject1);
	}
	else if (collisionType1 == CollisionHandlerType::Sphere && collisionType2 == CollisionHandlerType::Mesh)
	{
		return HandleSphereToMesh(contactInstance, contactsGenerated,
			(SphereCollider*)gameObject1->GetColliderObject(), glm::vec3(gameObject1->GetTransform()[3]),
			(MeshCollider*)gameObject2->GetColliderObject(), gameObject2->GetTransform(),
			restitution,
			gameObject1, gameObject2);
	}
	else if (collisionType1 == CollisionHandlerType::Mesh && collisionType2 == CollisionHandlerType::Sphere)
	{
		return HandleSphereToMesh(contactInstance, contactsGenerated,
			(SphereCollider*)gameObject2->GetColliderObject(), glm::vec3(gameObject2->GetTransform()[3]),
			(MeshCollider*)gameObject1->GetColliderObject(), gameObject1->GetTransform(),
			restitution,
			gameObject1, gameObject2);
	}
	else if (collisionType1 == CollisionHandlerType::Cuboid && collisionType2 == CollisionHandlerType::Cuboid)
	{
		return false;
	}
	else if (collisionType1 == CollisionHandlerType::Cuboid && collisionType2 == CollisionHandlerType::Mesh)
	{
		return false;
	}
	else if (collisionType1 == CollisionHandlerType::Mesh && collisionType2 == CollisionHandlerType::Cuboid)
	{
		return false;
	}
	else if (collisionType1 == CollisionHandlerType::Mesh && collisionType2 == CollisionHandlerType::Mesh)
	{
		return false;
	}
	else
	{
		printf("Collision handler not found!\n");
		return false;
	}
}

bool CollisionHandler::HandleSphereToSphere(ContactInstance * contactInstance, size_t& contactsGenerated,
	SphereCollider* sphereCollider1, const glm::vec3& position1,
	SphereCollider* sphereCollider2, const glm::vec3& position2,
	float restitution,
	GameObject* obj1, GameObject* obj2)
{
	glm::vec3 difference = position1 - position2;
	float radiSum = sphereCollider1->radius + sphereCollider2->radius;
	if (glm::dot(difference, difference) < (radiSum * radiSum)) // We collided!
	{
		contactInstance->contactNormal = glm::normalize(difference);
		contactInstance->obj1 = obj1;
		contactInstance->obj2 = obj2;
		contactInstance->penetration = glm::length(difference) - radiSum;
		contactInstance->restitution = restitution;
		contactInstance++;
		contactsGenerated++;
		return true;
	}

	return false;
}

bool CollisionHandler::HandleSphereToAABB(ContactInstance* contactInstance, size_t& contactsGenerated,
	SphereCollider* sphereCollider, const glm::vec3& spherePosition,
	AABBCollider* aabbCollider, const glm::mat4& aabbTransform,
	float restitution,
	GameObject* obj1, GameObject* obj2)
{
	float radius = sphereCollider->radius;

	glm::vec4 transformedMin = aabbTransform * glm::vec4(aabbCollider->min, 1.0f);
	glm::vec4 transformedMax = aabbTransform * glm::vec4(aabbCollider->max, 1.0f);

	// Find the closest AABB point to our particle
	float closestX = std::max(transformedMin.x, std::min(spherePosition.x, transformedMax.x));
	float closestY = std::max(transformedMin.y, std::min(spherePosition.y, transformedMax.y));
	float closestZ = std::max(transformedMin.z, std::min(spherePosition.z, transformedMax.z));

	glm::vec3 closestPoint(closestX, closestY, closestZ);
	glm::vec3 difference = spherePosition - closestPoint;

	// Since square roots are expensive, just compare everything in its squared version
	float distanceSquared = (difference.x * difference.x) + (difference.y * difference.y) + (difference.z * difference.z);
	float radiusSquared = radius * radius;

	if (distanceSquared < radiusSquared) // We have collided!
	{
		// Find contact normal
		glm::vec3 center = (aabbCollider->min + aabbCollider->max) / 2.0f;
		glm::vec3 normal = spherePosition - center;
		if (std::abs(normal.x) > std::abs(normal.y) && std::abs(normal.x) > std::abs(normal.z)) // Down the x axis
		{
			normal.y = 0.0f;
			normal.z = 0.0f;
		}
		else if (std::abs(normal.y) > std::abs(normal.x) && std::abs(normal.y) > std::abs(normal.z))
		{
			normal.x = 0.0f;
			normal.z = 0.0f;
		}
		else if (std::abs(normal.z) > std::abs(normal.y) && std::abs(normal.z) > std::abs(normal.x))
		{
			normal.x = 0.0f;
			normal.y = 0.0f;
		}
		glm::normalize(normal);

		contactInstance->contactNormal = normal;
		contactInstance->obj1 = obj1;
		contactInstance->obj2 = obj2;
		contactInstance->penetration = radius - sqrt(distanceSquared);
		contactInstance->restitution = restitution;
		contactInstance++;
		contactsGenerated++;
		return true;
	}

	return false;
}

bool CollisionHandler::HandleSphereToMesh(ContactInstance* contactInstance, size_t& contactsGenerated,
	SphereCollider* sphereCollider, const glm::vec3& spherePosition,
	MeshCollider* meshCollider, const glm::mat4& meshTransform,
	float restitution,
	GameObject* obj1, GameObject* obj2)
{
	const std::vector<MeshCollider::Vertex>& vertices = meshCollider->vertices;
	const std::vector<MeshCollider::Face>& indices = meshCollider->indices;

	for (int i = 0; i < indices.size(); i++)
	{
		const MeshCollider::Face& face = indices[i];
		const MeshCollider::Vertex& v1 = vertices[face.v1];
		const MeshCollider::Vertex& v2 = vertices[face.v2];
		const MeshCollider::Vertex& v3 = vertices[face.v3];

		glm::vec4 vertexPos1 = meshTransform * glm::vec4(v1.position, 1.0f);
		glm::vec4 vertexPos2 = meshTransform * glm::vec4(v2.position, 1.0f);
		glm::vec4 vertexPos3 = meshTransform * glm::vec4(v3.position, 1.0f);

		glm::mat3 rotation(meshTransform);
		glm::vec3 normal1 = glm::normalize(rotation * v1.normal);
		glm::vec3 normal2 = glm::normalize(rotation * v2.normal);
		glm::vec3 normal3 = glm::normalize(rotation * v3.normal);
		glm::vec3 normal = (normal1 + normal2 + normal3) / 3.0f; // Average 3 normals to get face normal

		float radius = sphereCollider->radius;

		glm::vec3 vertexPositionSum = vertexPos1 + vertexPos2 + vertexPos3;
		float distance = -glm::dot(vertexPositionSum / 3.0f, normal);
		float pointToPlaneDistance = glm::dot(normal, spherePosition) + distance;

		if (fabs(pointToPlaneDistance) > radius) // We are not breaking this polygon's infinite plane, no point in checking any further
		{
			continue;
		}

		// Check if our particle is within the face's triangle (https://blackpawn.com/texts/pointinpoly/default.html)
		auto IsSameSide = [](glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
		{
			return glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0.0f;
		};

		bool inside = IsSameSide(spherePosition, vertexPos1, vertexPos2, vertexPos3) 
			&& IsSameSide(spherePosition, vertexPos2, vertexPos1, vertexPos3) 
			&& IsSameSide(spherePosition, vertexPos3, vertexPos1, vertexPos2);

		if (!inside)
		{
			continue;
		}

		obj1->ResetJump();
		if (obj2) obj2->ResetJump();

		// We have made contact!
		contactInstance->contactNormal = normal;
		contactInstance->obj1 = obj1;
		contactInstance->obj2 = obj2;
		contactInstance->penetration = radius - pointToPlaneDistance;
		contactInstance->restitution = restitution;
		contactInstance++;
		contactsGenerated++;
		return true;
	}

	return false;
}