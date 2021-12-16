#pragma once
#include "Mesh.h"

#include <GameObject.h>
#include <ColliderObjects.h>

#include <string>
#include <yaml-cpp/yaml.h>

enum class ColliderType
{
	None = 0,
	Sphere,
	AABB,
	Mesh
};

class SaveableCollider
{
public:
	SaveableCollider(ColliderType colliderType, void* collider, float bounciness);
	~SaveableCollider() = default;

	virtual void Save(YAML::Emitter& emitter) const;
	static SaveableCollider StaticLoad(Ref<Mesh> mesh, const YAML::Node& node);

	ColliderType colliderType;
	void* collider;
	float bounciness;
};