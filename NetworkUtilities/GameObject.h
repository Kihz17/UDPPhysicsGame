#pragma once

#include <glm/glm.hpp>

#include <unordered_set>

enum class GameObjectType
{
	Player = 0,
	Cuboid,
	Sphere,
};

struct DirtyGameObject
{
	int id; // ID of the GameObject

	std::unordered_set<int> dirtyIndices;
};

class GameObject
{
public:
	GameObject(int id, const glm::mat4& transform, float bounciness, float mass) 
		: id(id), 
		transform(transform), 
		lastMoveRequestId(-1), 
		velocity(0.0f),
		bounciness(bounciness),
		acceleration(0.0f),
		damping(0.9f), 
		appliedForce(0.0f),
		dead(false),
		radius(1.0f)
	{
		SetMass(mass);
	}

	virtual ~GameObject() = default;

	inline virtual int GetId() { return id; }
	inline virtual const glm::mat4& GetTransformSafe() const { return transform; }
	inline virtual glm::mat4& GetTransform() { return transform; }

	inline virtual glm::vec3& GetVelocity() { return velocity; }
	inline virtual glm::vec3& GetAcceleration() { return acceleration; };

	virtual void SetMass(float mass);
	virtual float GetMass() const;
	inline virtual float GetInverseMass() const { return inverseMass; }

	virtual void SetDamping(float damping) { damping = glm::clamp(damping, 0.f, 1.f); };
	inline virtual float GetDamping() const { return damping; }

	virtual void ApplyForce(const glm::vec3& force);
	virtual void ApplyImpulse(const glm::vec3& impulse);
	inline void ApplyConstantForce(const glm::vec3& force) { constantAppliedForce = force; }

	virtual void ClearAppliedForces();

	virtual void Update(float deltaTime);

	virtual GameObjectType GetType() const = 0;

	virtual void ResetJump() {}

	inline void SetDead(bool dead) { this->dead = dead; }
	inline virtual bool isDead() const { return dead; }

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

	int lastMoveRequestId;
	float bounciness;
	float radius;

protected:
	int id;

	glm::mat4 transform;
	glm::vec3 velocity;

	float inverseMass;
	float damping;

	glm::vec3 acceleration;
	glm::vec3 appliedForce;
	glm::vec3 constantAppliedForce;

	bool dead;
};