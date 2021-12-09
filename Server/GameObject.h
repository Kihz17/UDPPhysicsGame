#pragma once

#include <glm/glm.hpp>

#include <unordered_set>
enum class CollisionHandlerType
{
	None = 0,
	Cuboid,
	Sphere,
	Mesh
};

struct DirtyGameObject
{
	int id; // ID of the GameObject

	std::unordered_set<int> dirtyIndices;
	// 0 = position
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;

	inline virtual int GetId() { return id; }
	inline virtual const glm::mat4& GetTransformSafe() const { return transform; }
	inline virtual glm::mat4& GetTransform() { return transform; }

	virtual CollisionHandlerType GetCollisionHandlerType() const = 0;

	inline virtual void UpdatePosition(const glm::vec3& position) 
	{ 
		transform[3].x = position.x;
		transform[3].y = position.y;
		transform[3].z = position.z;
	}

	inline virtual void UpdatePosition(float x, float y, float z)
	{
		transform[3].x = x;
		transform[3].y = y;
		transform[3].z = z;
	}

	static int GetNextGameObjectId();

private:
	int id;
	glm::mat4 transform;
};