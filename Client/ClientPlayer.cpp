#include "ClientPlayer.h"

#include <PlayerInfo.h>
#include <glm/gtc/matrix_transform.hpp>

ClientPlayer::ClientPlayer(int id, const glm::mat4& transform)
	: ClientGameObject(id, transform, PlayerInfo::PlayerDefaultBounciness, PlayerInfo::PlayerMass),
	sphereCollider(new SphereCollider())
{
	this->transform *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	this->sphereCollider->radius = PlayerInfo::PlayerColliderRadius;
}

ClientPlayer::~ClientPlayer()
{
	delete sphereCollider;
}