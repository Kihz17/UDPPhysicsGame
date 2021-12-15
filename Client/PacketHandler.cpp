#include "PacketHandler.h"
#include "Client.h"
#include "Input.h"

#include <Packets.h>

// 0 = PacketPlayerInput (Client -> Server & Server -> Client)
// 1 = PacketPlayerConnectSuccess (Server -> Client)
// 2 = PacketSpawnGameObject (Server -> Client)
// 3 = PacketDestroyGameObject (Server -> Client)
// 4 = PacketUpdateGameObjectPositions (Server -> Client)
// 5 = PacketServerShutdown (Server -> Client)
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Client* client)
{
	if (packetType == 1)
	{
		PacketPlayerConnectSuccess packet(buffer);
		if (client->ourId != -1)
		{
			printf("Received our GameObject ID more than once!\n");
		}

		client->ourId = packet.playerId;
		client->connected = true;
		printf("Our GameObject ID was successfully retreived from the Server!\n");
		return;
	}
	else if (packetType == 2)
	{
		PacketSpawnGameObject packet(buffer);
		if (client->ourId != -1 && !client->playerController) // We are spawning ourselves in the world, setup player controller
		{
			client->playerController = new PlayerController(client->ourId, client->camera, glm::vec3(packet.x, packet.y, packet.z));
			Input::SetCursorMode(CursorMode::Locked);
		}
		else
		{
			client->world->AddGameObject(packet.gameObjectId, packet.type, packet.x, packet.y, packet.z); // Add this gameobject to the world
		}
		return;
	}
	else if (packetType == 3)
	{
		PacketRemoveGameObject packet(buffer);
		client->world->RemoveGameObject(packet.gameObjectId);
		return;
	}
	else if (packetType == 4)
	{
		PacketUpdateGameObjectPositions packet(buffer);
		for (const GameObjectMoveUpdate& update : packet.data)
		{
			bool isPlayer = update.gameObjectId == client->ourId;
			if (isPlayer) // We are ourselves
			{
				client->playerController->ValidateMoveState(update.requestId, update.position);
			}
			else // We are some different game object
			{
				ClientGameObject* gameObject = client->world->GetGameObject(update.gameObjectId);
				glm::vec3 oldPosition = glm::vec3(gameObject->GetTransform()[3]);
				gameObject->GetVelocity() = update.velocity; // Update velocity with server's state
				gameObject->lastKnownPosition = update.position;
				gameObject->ValidateMoveState(update.requestId, update.position); // Make sure this gameobject is in the right position, if not correct it
				gameObject->timeSinceLastUpdate = 0.00001f; // We jsut received a game state update, update
			}
		}
		return;
	}
	else if (packetType == 5)
	{
		client->connected = false;
		printf("Server has shutdown.\n");
		return;
	}

	printf("No handler for PacketType %d", packetType);
}