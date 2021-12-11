#pragma once
#include "Mesh.h"

#include <GameObject.h>
#include <string>
#include <yaml-cpp/yaml.h>

class SaveableCollider
{
public:
	SaveableCollider(CollisionHandlerType colliderType, void* collider, float bounciness);
	~SaveableCollider() = default;

	virtual void Save(YAML::Emitter& emitter) const;
	static SaveableCollider StaticLoad(Ref<Mesh> mesh, const YAML::Node& node);

	CollisionHandlerType colliderType;
	void* collider;
	float bounciness;
};