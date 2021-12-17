#include "PlayerInfo.h"

const float PlayerInfo::PlayerMoveSpeed = 100.0f;
const float PlayerInfo::MaximumPositionLeniency = 1.0f; // 2 units
const float PlayerInfo::DecelerationRate = 0.9f;
const glm::vec3 PlayerInfo::Gravity = glm::vec3(0.0f, -80.0f, 0.0f);
const float PlayerInfo::PlayerMass = 2.0f;
const float PlayerInfo::PlayerDefaultBounciness = 0.01f;
const float PlayerInfo::PlayerColliderRadius = 2.0f;
const float PlayerInfo::PlayerJumpForce = 50.0f;
const float PlayerInfo::PlayerDamping = 0.1f;
const float PlayerInfo::PlayerRadius = 2.0f;
const float PlayerInfo::HitForce = 20.0f;
const float PlayerInfo::MaxHitDistance = 200.0f;