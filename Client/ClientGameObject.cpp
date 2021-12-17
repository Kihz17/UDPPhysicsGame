#include "ClientGameObject.h"
#include <PlayerInfo.h>

#include <iostream>

ClientGameObject::ClientGameObject(int id, const glm::mat4 & transform, float bounciness, float mass)
	: GameObject(id, transform, bounciness, mass),
	timeSinceLastUpdate(0.0f), 
	lastKnownPosition(0.0f)
{
	this->radius = PlayerInfo::PlayerRadius;
	this->damping = PlayerInfo::PlayerDamping;
}

void ClientGameObject::ValidateMoveState(int requestId, const glm::vec3& position, bool lerp)
{
	if (lerp)
	{
		glm::vec3 currentPosition = glm::vec3(GetTransform()[3]);
		glm::vec3 difference = position - currentPosition;
		if (glm::dot(difference, difference) > PlayerInfo::MaximumPositionLeniency) // We aren't where we are supposed to be
		{
			std::cout << "Making correction\n";
			shouldLerp = true;
			lerpTo = position;
			lerpIncrement = std::min(1.0f, glm::distance(currentPosition, position) / 1000.0f);
			lerpValue = lerpIncrement;
		}
	}
	else
	{
		UpdatePosition(position);
	}
}

void ClientGameObject::Update(float deltaTime)
{
	timeSinceLastUpdate += deltaTime;
	if (shouldLerp)
	{
		if (lerpValue < 1.0f)
		{
			std::cout << "Lerping with increment " << lerpIncrement << std::endl;
			glm::vec3 lerped = glm::mix(glm::vec3(this->transform[3]), lerpTo, lerpValue);
			this->transform[3].x = lerped.x;
			this->transform[3].y = lerped.y;
			this->transform[3].z = lerped.z;
			lerpValue += lerpIncrement;
			return;
		}
		else
		{
			std::cout << "Done lerping\n";
			shouldLerp = false;
		}
	}

	this->transform[3].x = this->lastKnownPosition.x + this->velocity.x * timeSinceLastUpdate * 16.0f;
	this->transform[3].y = this->lastKnownPosition.y + this->velocity.y * timeSinceLastUpdate * 16.0f;
	this->transform[3].z = this->lastKnownPosition.z + this->velocity.z * timeSinceLastUpdate * 16.0f;
}