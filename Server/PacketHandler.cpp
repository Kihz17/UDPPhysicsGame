#include "PacketHandler.h"
#include "Server.h"

#include <PlayerInfo.h>
#include <Packets.h>

#include <iostream>

// 0 = PacketPlayerInput (Client -> Server & Server -> Client)
// 1 = PacketPlayerConnectSuccess (Server -> Client)
// 2 = PacketSpawnGameObject (Server -> Client)
// 3 = PacketDestroyGameObject (Server -> Client)
// 4 = PacketUpdateGameObjectPositions (Server -> Client)
// 5 = PacketServerShutdown (Server -> Client)
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Server* server)
{
	if (packetType == 0) // Player moved
	{
		PacketPlayerInput packet(buffer);
		GameObject* gameObject = server->GetGameObject(packet.gameObjectId);
		if (packet.requestId < gameObject->lastMoveRequestId) return; // This input is no longer valid

		gameObject->lastMoveRequestId = packet.requestId; // Update request to now
		glm::mat4& transform = gameObject->GetTransform();
		glm::vec3 newPosition = packet.position;
		glm::vec3 oldPosition = glm::vec3(transform[3]);;
		glm::vec3 velocity = newPosition - oldPosition;
		float distanceSqr = glm::dot(velocity, velocity);
		if (distanceSqr > PlayerInfo::MaximumPositionLeniency) // We have moved too far in one move, correct this
		{
			velocity = glm::normalize(velocity);
			gameObject->GetVelocity() = velocity;
			newPosition = oldPosition + velocity;
			gameObject->UpdatePosition(newPosition.x, newPosition.y, newPosition.z);
			return;
		}

		// TODO: Do some physics check to see if we can move to the position, if not, correct

		gameObject->GetVelocity() = velocity;
		gameObject->UpdatePosition(newPosition.x, newPosition.y, newPosition.z);
		return;
	}

	printf("No handler for PacketType %d\n", packetType);
}