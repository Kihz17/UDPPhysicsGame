#pragma once

#include "GameObject.h"

// Represents a collision between 2 game objects
class ContactInstance
{
public:
	float CalculateSeparatingVelocity() const;
	void Resolve(float deltaTime);

	GameObject* obj1; // The first object involved in this contact
	GameObject* obj2; // The second object involved in this contact (can be NULL)

	glm::vec3 contactNormal; // The direction of the contact
	float restitution;
	float penetration; // How deep did we penetrate whatever we are colliding into?

	// The amount the particle will be moved when resolving positions
	glm::vec3 movement1;
	glm::vec3 movement2;
};