#include "GameObject.h"

void GameObject::SetMass(float mass)
{
	if (mass <= 0.f)
	{
		this->inverseMass = 0.f;
	}
	else
	{
		this->inverseMass = 1.f / mass;
	}
}

float GameObject::GetMass() const
{
	if (this->inverseMass == 0.f)
	{
		return this->inverseMass;
	}
	return 1.0f / this->inverseMass;
}

void GameObject::ApplyForce(const glm::vec3& force)
{
	this->appliedForce += force;
}

void GameObject::ApplyImpulse(const glm::vec3& impulse)
{
	this->velocity += impulse * this->inverseMass;
}

void GameObject::ClearAppliedForces()
{
	this->appliedForce.x = 0.0f;
	this->appliedForce.y = 0.0f;
	this->appliedForce.z = 0.0f;

	this->constantAppliedForce.x = 0.0f;
	this->constantAppliedForce.y = 0.0f;
	this->constantAppliedForce.z = 0.0f;
}

void GameObject::Update(float deltaTime)
{
	if (this->inverseMass == 0.f)
	{
		return;
	}

	velocity += constantAppliedForce;

	this->transform[3].x += this->velocity.x * deltaTime;
	this->transform[3].y += this->velocity.y * deltaTime;
	this->transform[3].z += this->velocity.z * deltaTime;

	this->velocity += (this->acceleration + this->appliedForce * this->inverseMass) * deltaTime;
	this->velocity *= glm::pow(this->damping, deltaTime);

	ClearAppliedForces();
}