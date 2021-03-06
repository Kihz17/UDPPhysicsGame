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
	static const float PlayerJumpForce;
	static const float PlayerDamping;
	static const float PlayerRadius;
	static const float HitForce;
	static const float MaxHitDistance;
};
