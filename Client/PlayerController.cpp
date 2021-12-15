#include "PlayerController.h"
#include "Input.h"
#include "GLCommon.h"

#include <PlayerInfo.h>
#include <iostream>

static const float cameraYOffset = 5.0f;

PlayerController::PlayerController(int id, Ref<Camera> camera, const glm::vec3& position)
	: ClientPlayer(id, glm::mat4(1.0f)),
	camera(camera), 
	handleInput(true), 
	requestId(0),
	lastSpacePress(10000.0f),
	jumpCount(0)
{
	transform[3].x = position.x;
	transform[3].y = position.y;
	transform[3].z = position.z;
}

void PlayerController::OnUpdate(float deltaTime)
{
	lastSpacePress += deltaTime;

	if (handleInput)
	{
		camera->MoveCamera(Input::GetMouseX(), Input::GetMouseY());

		glm::vec3 movementDirection = camera->direction;
		movementDirection.y = 0.0f;
		movementDirection = glm::normalize(movementDirection);

		glm::vec3 force(0.0f, 0.0f, 0.0f);

		glm::vec3 newPosition = glm::vec3(transform[3]);
		if (Input::IsKeyPressed(Key::W))
		{
			force += movementDirection * PlayerInfo::PlayerMoveSpeed;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			force += movementDirection * -PlayerInfo::PlayerMoveSpeed;
		}

		if (Input::IsKeyPressed(Key::A))
		{
			constexpr float theta = glm::radians(90.0f);
			glm::vec3 left(movementDirection.x * cos(theta) + movementDirection.z * sin(theta), 0.0f, -movementDirection.x * sin(theta) + movementDirection.z * cos(theta));
			force += left * PlayerInfo::PlayerMoveSpeed;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			constexpr float theta = glm::radians(-90.0f);
			glm::vec3 right(movementDirection.x * cos(theta) + movementDirection.z * sin(theta), 0.0f, -movementDirection.x * sin(theta) + movementDirection.z * cos(theta));
			force += right * PlayerInfo::PlayerMoveSpeed;
		}

		bool spacePressed = Input::IsKeyPressed(Key::Space);
	/*	if (spacePressed)
		{
			std::cout << "Space\n";
		}*/

		if (jumpCount < 2 && Input::IsKeyPressed(Key::Space) && lastSpacePress >= 0.5f)
		{
			lastSpacePress = 0.0f;
			force.y += PlayerInfo::PlayerJumpForce;
    		jumpCount++;
		}

		//std::cout << "SpacePress: " << lastSpacePress << std::endl;
		//std::cout << "X: " << force.x << "Y: " << force.y << "Z: " << force.z << std::endl;
		ApplyForce(force); // Apply force from player controls
	}

	ApplyForce(PlayerInfo::Gravity * GetMass()); // Apply gravity

	Update(deltaTime); // Move
}

void PlayerController::ToggleHandleInput(bool state)
{
	handleInput = state;
	if (handleInput)
	{
		Input::SetCursorMode(CursorMode::Locked);
	}
	else
	{
		Input::SetCursorMode(CursorMode::Normal);
	}
}

void PlayerController::RemoveMoveStateAt(int index)
{
	std::map<int, ClientMoveState>::iterator it = moveStates.begin();
	while (it != moveStates.end())
	{
		if (it->first <= index)
		{
			it = moveStates.erase(it);
		}
		else
		{
			break;
		}
	}
}

void PlayerController::ValidateMoveState(int requestId, const glm::vec3& position)
{
	glm::vec3 oldPosition = glm::vec3(GetTransform()[3]);

	std::map<int, ClientMoveState>::iterator it = moveStates.find(requestId);
	if (it == moveStates.end()) return; // Our client doesn't contain a move state for this requestId, we have probably already processed it

	ClientMoveState& moveState = it->second;
	glm::vec3 difference = position - moveState.postion;
	if (glm::dot(difference, difference) > PlayerInfo::MaximumPositionLeniency) // Server is telling us we are not where we are supposed to be
	{
		UpdatePosition(position);
	}

	RemoveMoveStateAt(requestId); // Remove this state and all states before it
}