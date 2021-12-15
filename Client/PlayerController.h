#pragma once

#include "pch.h"
#include "Camera.h"
#include "ClientPlayer.h"

#include <glm/glm.hpp>

class PlayerController : public ClientPlayer
{
public:
	PlayerController(int id, Ref<Camera> camera, const glm::vec3& position);
	virtual ~PlayerController() = default;

	void OnUpdate(float deltaTime);

	void ToggleHandleInput(bool state);

	virtual void ValidateMoveState(int requestId, const glm::vec3& position) override;
	virtual void RemoveMoveStateAt(int index);
	inline virtual void InsertMoveState(int requestId, ClientMoveState moveState) { moveStates.insert({ requestId, moveState }); }

	inline glm::vec3& GetLastNormalizedVelocity() { return lastNormalizedVelocity; }
	inline int NextMoveRequestId() { return requestId++; }

	inline virtual void ResetJump() override { jumpCount = 0; }
	inline virtual GameObjectType GetType() const override { return GameObjectType::Player; }
	inline virtual CollisionHandlerType GetCollisionHandlerType() const override { return CollisionHandlerType::Sphere; }

private:
	friend class Client;

	Ref<Camera> camera;
	bool handleInput;
	glm::vec3 lastNormalizedVelocity;
	int requestId;

	float lastSpacePress;
	int jumpCount;

	std::map<int, ClientMoveState> moveStates;
};