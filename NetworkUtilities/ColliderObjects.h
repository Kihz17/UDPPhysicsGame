#pragma once

#include <glm/glm.hpp>

#include <vector>

struct SphereCollider
{
	SphereCollider(float radius) : radius(radius) {}
	float radius;
};

struct AABBCollider
{
	AABBCollider(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
	glm::vec3 min, max;
};

struct MeshCollider
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	struct Face
	{
		uint32_t v1, v2, v3;
	};

	MeshCollider(std::vector<Vertex> vertices, std::vector<Face> indces) : vertices(vertices), indces(indces) {}

	std::vector<Vertex> vertices;
	std::vector<Face> indces;
};