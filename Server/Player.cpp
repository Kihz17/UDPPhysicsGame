#include "Player.h"

Player::Player(unsigned short port, sockaddr_in from, const glm::mat4& transform)
{
	networkInfo.port = port;
	networkInfo.fromSocket = from;
	this->transform = transform;
}

Player::~Player()
{
	
}