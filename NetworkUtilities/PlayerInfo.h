#pragma once

#include <glm/glm.hpp>

class PlayerInfo
{
public:
	static const float PlayerMoveSpeed;
	static const float MaximumPositionLeniency;
	static const float DecelerationRate; // USed for when an objects velocity is > PlayerMoveSpeed
	static const glm::vec3 Gravity;
	static const float PlayerMass;
	static const float PlayerDefaultBounciness;
	static const float PlayerColliderRadius;
};
