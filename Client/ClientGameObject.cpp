#include "ClientGameObject.h"
#include <PlayerInfo.h>
ClientGameObject::ClientGameObject(int id, const glm::mat4 & transform, float bounciness, float mass)
	: GameObject(id, transform, bounciness, mass),
	timeSinceLastUpdate(0.0f), 
	lastKnownPosition(0.0f)
{

}

void ClientGameObject::ValidateMoveState(int requestId, const glm::vec3& position)
{
	glm::vec3 currentPosition = glm::vec3(GetTransform()[3]);
	glm::vec3 difference = position - currentPosition;
	if (glm::dot(difference, difference) > PlayerInfo::MaximumPositionLeniency) // We aren't where we are supposed to be
	{
		UpdatePosition(position);
	}
}