#include "ClientPlayer.h"

#include <PlayerInfo.h>
#include <glm/gtc/matrix_transform.hpp>

ClientPlayer::ClientPlayer(int id, const glm::mat4& transform)
	: ClientGameObject(id, transform, PlayerInfo::PlayerDefaultBounciness, PlayerInfo::PlayerMass)
{

}

ClientPlayer::~ClientPlayer()
{
	
}