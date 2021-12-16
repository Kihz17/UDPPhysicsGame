#include "SaveableCollider.h"
#include "YAMLOverloads.h"

#include <iostream>

static std::string ColliderTypeToString(ColliderType colliderType)
{
	switch (colliderType)
	{
	case ColliderType::Sphere:	return "Sphere";
	case ColliderType::AABB:	return "AABB";
	case ColliderType::Mesh:	return "Mesh";
	default:
		std::cout << "No conversion for collider type " << (int) colliderType << std::endl;
		break;
	}
}

static ColliderType ColliderTypeFromString(const std::string& s)
{
	if (s == "Sphere")		return ColliderType::Sphere;
	else if(s == "AABB")	return ColliderType::AABB;
	else if (s == "Mesh")	return ColliderType::Mesh;

	std::cout << "No string conversion for collider type " << s << std::endl;
	return ColliderType::None;
}

SaveableCollider::SaveableCollider(ColliderType colliderType, void* collider, float bounciness)
	: colliderType(colliderType), collider(collider), bounciness(bounciness)
{

}

void SaveableCollider::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "ColliderType" << YAML::Value << ColliderTypeToString(colliderType);
	emitter << YAML::Key << "Bounciness" << YAML::Value << bounciness;
	if (colliderType == ColliderType::Sphere)
	{
		emitter << YAML::Key << "Radius" << YAML::Value << ((SphereCollider*)collider)->radius;
	}
	else if (colliderType == ColliderType::AABB)
	{
		emitter << YAML::Key << "Min" << YAML::Value << ((AABBCollider*)collider)->min;
		emitter << YAML::Key << "Max" << YAML::Value << ((AABBCollider*)collider)->max;
	}
	else if (colliderType == ColliderType::Mesh)
	{
		// No need to save anything here because the data we need will be in a mesh object
	}
	emitter << YAML::EndMap;
}

SaveableCollider SaveableCollider::StaticLoad(Ref<Mesh> mesh, const YAML::Node& node)
{
	ColliderType colliderType = ColliderTypeFromString(node["ColliderType"].as<std::string>());
	float bounciness = node["Bounciness"].as<float>();

	if (colliderType == ColliderType::Sphere)
	{
		float radius = node["Radius"].as<float>();
		SphereCollider* sphereCollider = new SphereCollider(radius);
		SaveableCollider collider(colliderType, sphereCollider, bounciness);
		return collider;
	}
	else if (colliderType == ColliderType::AABB)
	{
		glm::vec3 min = node["Min"].as<glm::vec3>();
		glm::vec3 max = node["Max"].as<glm::vec3>();

		AABBCollider* aabbCollider = new AABBCollider(min, max);
		SaveableCollider collider(colliderType, aabbCollider, bounciness);
		return collider;
	}
	else if (colliderType == ColliderType::Mesh)
	{
		std::vector<MeshCollider::Vertex> vertices;
		for (const Vertex& vertex : mesh->GetVertices())
		{
			MeshCollider::Vertex vertexToAdd;
			vertexToAdd.position = vertex.position;
			vertexToAdd.normal = vertex.normal;
			vertices.push_back(vertexToAdd);
		}

		std::vector<MeshCollider::Face> indices;
		for (const Face& face : mesh->GetFaces())
		{
			MeshCollider::Face faceToAdd;
			faceToAdd.v1 = face.v1;
			faceToAdd.v2 = face.v2;
			faceToAdd.v3 = face.v3;
			indices.push_back(faceToAdd);
		}

 		MeshCollider* meshCollider = new MeshCollider(vertices, indices);
		SaveableCollider collider(colliderType, meshCollider, bounciness);
		return collider;
	}

	std::cout << "Could not load SaveableCollider" << std::endl;
	return SaveableCollider(ColliderType::None, nullptr, 0.0f);
}