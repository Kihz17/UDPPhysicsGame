#include "PacketHandler.h"
#include "Server.h"

#include <PlayerInfo.h>
#include <Packets.h>

#include <iostream>
#include <chrono>

// 0 = PacketPlayerInput (Client -> Server & Server -> Client)
// 1 = PacketPlayerConnectSuccess (Server -> Client)
// 2 = PacketSpawnGameObject (Server -> Client)
// 3 = PacketDestroyGameObject (Server -> Client)
// 4 = PacketUpdateGameObjectPositions (Server -> Client)
// 5 = PacketServerShutdown (Server -> Client)
// 5 = PacketReadyUp (Client -> Server)
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Server* server, Player* player)
{
	if (packetType == 0) // Player moved
	{
		PacketPlayerInput packet(buffer);
		GameObject* gameObject = server->GetGameObject(packet.gameObjectId);
		if (packet.requestId < gameObject->lastMoveRequestId) return; // This input is no longer valid

		gameObject->lastMoveRequestId = packet.requestId; // Update request to now
		glm::mat4& transform = gameObject->GetTransform();
		glm::vec3 newPosition = packet.position;
		glm::vec3 oldPosition = glm::vec3(transform[3]);
		glm::vec3 velocity = newPosition - oldPosition;

		glm::vec3 horizontalDifference = glm::vec3(newPosition.x, 0.0f, newPosition.z) - glm::vec3(oldPosition.x, 0.0f, oldPosition.z);
		float horizontalDistanceSqr = glm::dot(horizontalDifference, horizontalDifference);

		glm::vec3 verticalDifference = glm::vec3(0.0f, newPosition.y, 0.0f) - glm::vec3(0.0f, oldPosition.y, 0.0f);
		float verticalDistanceSqr = glm::dot(verticalDifference, verticalDifference);

		time_t lastInputUpdate = player->lastInputTime;
		time_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		time_t millisPassed = now - lastInputUpdate;

		float millis = ((float)millisPassed / 1500.0f);
		float horiztonalLeniencey = PlayerInfo::PlayerMoveSpeed * millis;

		player->lastInputTime = now;

		glm::vec3 direction = glm::normalize(velocity);
	
		if (horizontalDistanceSqr > horiztonalLeniencey) // We have travelled an unrealistic distance horizontally
		{
			// Correct horizontal position
			newPosition.x = oldPosition.x + (direction.x * PlayerInfo::PlayerMoveSpeed * millis);
			newPosition.z = oldPosition.z + (direction.z * PlayerInfo::PlayerMoveSpeed * millis);

			// Correct velocity
			velocity = newPosition - oldPosition;
		}

		if (velocity.y < 0.0f) // We are being affected by gravity
		{
			float verticalLeniencey = PlayerInfo::Gravity.y * millis;
			if (velocity.y < PlayerInfo::Gravity.y) // We are moving faster than gravity is pulling us down
			{
				newPosition.y = oldPosition.y - verticalLeniencey;
				velocity.y = verticalLeniencey;
			}
		}
		else // Probably jumping
		{
			float verticalLeniencey = PlayerInfo::PlayerJumpForce;
		
			if (verticalDistanceSqr > verticalLeniencey) // We have travelled an unrealistic distance vertically
			{
				// Correct vertical position
				newPosition.y = oldPosition.y + direction.y * PlayerInfo::PlayerJumpForce * ((float)millisPassed / 1000.0f);

				// Correct velocity
				velocity = newPosition - oldPosition;
			}
		}
		

		// TODO: Do some physics check to see if we can move to the position, if not, correct

		gameObject->GetVelocity() = velocity;
		gameObject->UpdatePosition(newPosition.x, newPosition.y, newPosition.z);
		return;
	}
	else if (packetType == 6) // Ready up packet
	{
		if (server->gameOver)
		{
			server->readiedPlayers.insert(player);
			printf("Player %d has readied.\n", player->GetId());
		}
		return;
	}

	printf("No handler for PacketType %d\n", packetType);
}