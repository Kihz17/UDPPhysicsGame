#pragma once

#include <GameObject.h>
#include <map>

struct ClientMoveState
{
	glm::vec3 postion;
};

class ClientGameObject : public GameObject
{
public:
	ClientGameObject(int id, const glm::mat4& transform, float bounciness, float mass);
	virtual ~ClientGameObject() = default;

	virtual void ValidateMoveState(int requestId, const glm::vec3& position);

	float timeSinceLastUpdate;
	glm::vec3 lastKnownPosition;
};