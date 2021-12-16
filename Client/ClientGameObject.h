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

	virtual void Update(float deltaTime) override;

	float timeSinceLastUpdate;
	glm::vec3 lastKnownPosition;
	bool shouldLerp = false;
	glm::vec3 lerpTo;
	float lerpIncrement;
	float lerpValue;
};