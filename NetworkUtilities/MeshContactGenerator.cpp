#include "MeshContactGenerator.h"

MeshContactGenerator::MeshContactGenerator(MeshCollider* collider, const glm::mat4& transform, float bounciness)
	: collider(collider),
	transform(transform),
	bounciness(bounciness)
{

}

MeshContactGenerator::~MeshContactGenerator()
{
	delete collider;
}

size_t MeshContactGenerator::AddContact(std::vector<GameObject*>& gameObjects, ContactInstance* contactInstance, size_t contactLimit) const
{
	size_t contactsGenerated = 0;
	for (GameObject* gameObject : gameObjects)
	{
		glm::vec3 particlePos = glm::vec3(gameObject->GetTransform()[3]);
		float radius = gameObject->radius;
		float radius2 = radius * radius;

		for (int i = 0; i < collider->indces.size(); i++)
		{
			const MeshCollider::Face& face = collider->indces[i];
			const MeshCollider::Vertex& v1 = collider->vertices[face.v1];
			const MeshCollider::Vertex& v2 = collider->vertices[face.v2];
			const MeshCollider::Vertex& v3 = collider->vertices[face.v3];

			glm::vec4 vertexPos1 = transform * glm::vec4(v1.position, 1.0f);
			glm::vec4 vertexPos2 = transform * glm::vec4(v2.position, 1.0f);
			glm::vec4 vertexPos3 = transform * glm::vec4(v3.position, 1.0f);

			glm::mat3 rotation(transform);
			glm::vec3 normal1 = glm::normalize(rotation * v1.normal);
			glm::vec3 normal2 = glm::normalize(rotation * v2.normal);
			glm::vec3 normal3 = glm::normalize(rotation * v3.normal);
			glm::vec3 normal = (normal1 + normal2 + normal3) / 3.0f; // Average 3 normals to get face normal

			glm::vec3 vertexPositionSum = vertexPos1 + vertexPos2 + vertexPos3;
			float distance = -glm::dot(vertexPositionSum / 3.0f, normal);
			float pointToPlaneDistance = glm::dot(normal, particlePos) + distance;

			if (fabs(pointToPlaneDistance) > radius) // We are not breaking this polygon's infinite plane, no point in checking any further
			{
				continue;
			}

			// Check if our particle is within the face's triangle (https://blackpawn.com/texts/pointinpoly/default.html)
			auto IsSameSide = [](glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
			{
				return glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0.0f;
			};

			bool inside = IsSameSide(particlePos, vertexPos1, vertexPos2, vertexPos3) && IsSameSide(particlePos, vertexPos2, vertexPos1, vertexPos3) && IsSameSide(particlePos, vertexPos3, vertexPos1, vertexPos2);
			if (!inside)
			{
				continue;
			}

			// We have made contact!
			contactInstance->contactNormal = normal;
			contactInstance->obj1 = gameObject;
			contactInstance->obj2 = NULL;
			contactInstance->penetration = radius - pointToPlaneDistance;
			contactInstance->restitution = bounciness + gameObject->bounciness;
			contactInstance++;
			contactsGenerated++;
			if (contactsGenerated > contactLimit)
			{
				return contactsGenerated;
			}

			break; // We have already collided, no point in continuing to look
		}
	}

	return contactsGenerated;
}