#pragma once

#include <glm/glm.hpp>

#include <vector>

struct SphereCollider
{
	float radius;
};

struct AABBCollider
{
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

	std::vector<Vertex> vertices;
	std::vector<Face> indices;
};