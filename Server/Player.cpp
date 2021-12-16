
#include "Player.h"
#include <PlayerInfo.h>

Player::Player(int id, unsigned short port, sockaddr_in from, const glm::mat4& transform)
	: GameObject(id, transform, PlayerInfo::PlayerDefaultBounciness, PlayerInfo::PlayerMass)
{
	networkInfo.port = port;
	networkInfo.fromSocket = from;
}

Player::~Player()
{
	
}