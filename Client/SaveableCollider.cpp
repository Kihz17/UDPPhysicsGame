#include "SaveableCollider.h"
#include "YAMLOverloads.h"

#include <CollisionObjects.h>
#include <iostream>

static std::string ColliderTypeToString(CollisionHandlerType colliderType)
{
	switch (colliderType)
	{
	case CollisionHandlerType::Sphere:	return "Sphere";
	case CollisionHandlerType::Cuboid:	return "Cuboid";
	case CollisionHandlerType::Mesh:	return "Mesh";
	default:
		std::cout << "No conversion for collider type " << (int) colliderType << std::endl;
		break;
	}
}

static CollisionHandlerType ColliderTypeFromString(const std::string& s)
{
	if (s == "Sphere")		return CollisionHandlerType::Sphere;
	else if(s == "Cuboid")	return CollisionHandlerType::Cuboid;
	else if (s == "Mesh")	return CollisionHandlerType::Mesh;

	std::cout << "No string conversion for collider type " << s << std::endl;
	return CollisionHandlerType::None;
}

SaveableCollider::SaveableCollider(CollisionHandlerType colliderType, void* collider, float bounciness)
	: colliderType(colliderType), collider(collider), bounciness(bounciness)
{

}

void SaveableCollider::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "ColliderType" << YAML::Value << ColliderTypeToString(colliderType);
	emitter << YAML::Key << "Bounciness" << YAML::Value << bounciness;
	if (colliderType == CollisionHandlerType::Sphere)
	{
		emitter << YAML::Key << "Radius" << YAML::Value << ((SphereCollider*)collider)->radius;
	}
	else if (colliderType == CollisionHandlerType::Cuboid)
	{
		emitter << YAML::Key << "Min" << YAML::Value << ((AABBCollider*)collider)->min;
		emitter << YAML::Key << "Max" << YAML::Value << ((AABBCollider*)collider)->max;
	}
	else if (colliderType == CollisionHandlerType::Mesh)
	{
		// No need to save anything here because the data we need will be in a mesh object
	}
	emitter << YAML::EndMap;
}

SaveableCollider SaveableCollider::StaticLoad(Ref<Mesh> mesh, const YAML::Node& node)
{
	CollisionHandlerType colliderType = ColliderTypeFromString(node["ColliderType"].as<std::string>());
	float bounciness = node["Bounciness"].as<float>();

	if (colliderType == CollisionHandlerType::Sphere)
	{
		float radius = node["Radius"].as<float>();
		SphereCollider* sphereCollider = new SphereCollider();
		sphereCollider->radius = radius;
		SaveableCollider collider(colliderType, sphereCollider, bounciness);
		return collider;
	}
	else if (colliderType == CollisionHandlerType::Cuboid)
	{
		glm::vec3 min = node["Min"].as<glm::vec3>();
		glm::vec3 max = node["Max"].as<glm::vec3>();

		AABBCollider* aabbCollider = new AABBCollider();
		aabbCollider->min = min;
		aabbCollider->max = max;
		SaveableCollider collider(colliderType, aabbCollider, bounciness);
		return collider;
	}
	else if (colliderType == CollisionHandlerType::Mesh)
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

 		MeshCollider* meshCollider = new MeshCollider();
		meshCollider->vertices = vertices;
		meshCollider->indices = indices;
		SaveableCollider collider(colliderType, meshCollider, bounciness);
		return collider;
	}

	std::cout << "Could not load SaveableCollider" << std::endl;
	return SaveableCollider(CollisionHandlerType::None, nullptr, 0.0f);
}