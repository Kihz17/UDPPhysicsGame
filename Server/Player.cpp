
#include "Player.h"
#include <PlayerInfo.h>

Player::Player(int id, unsigned short port, sockaddr_in from, const glm::mat4& transform)
	: GameObject(id, transform, PlayerInfo::PlayerDefaultBounciness, PlayerInfo::PlayerMass),
	collider(new SphereCollider())
{
	networkInfo.port = port;
	networkInfo.fromSocket = from;
	collider->radius = PlayerInfo::PlayerColliderRadius;
}

Player::~Player()
{
	delete collider;
}