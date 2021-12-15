#include "PlayerInfo.h"

const float PlayerInfo::PlayerMoveSpeed = 25.0f;
const float PlayerInfo::MaximumPositionLeniency = 4.0f; // 2 units
const float PlayerInfo::DecelerationRate = 0.9f;
const glm::vec3 PlayerInfo::Gravity = glm::vec3(0.0f, -20.0f, 0.0f);
const float PlayerInfo::PlayerMass = 2.0f;
const float PlayerInfo::PlayerDefaultBounciness = 0.01f;
const float PlayerInfo::PlayerColliderRadius = 2.0f;
const float PlayerInfo::PlayerJumpForce = 6000.0f;