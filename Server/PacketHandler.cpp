#include "PacketHandler.h"
#include "Server.h"

#include <PacketUpdatePosition.h>

// 0 = PacketUpdatePosition (Client -> Server & Server -> Client)
// 1 = 
// 2 = PacketSpawnGameObject (Server -> Client)
// 3 = PacketDestroyGameObject (Server -> Client)
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Server* server)
{
	printf("Handling PacketType %d...\n", packetType);
	if (packetType == 0)
	{
		PacketUpdatePosition positionPacket(buffer);
		GameObject* gameObject = server->GetGameObject(positionPacket.gameObjectId);
		gameObject->UpdatePosition(positionPacket.x, positionPacket.y, positionPacket.z);
		server->FlagDirty(gameObject, 0);
		return;
	}

	// Packets:
// PacketUpdatePosition C 
// PacketUpdateOrientation
// PacketUpdateVelocity
// PacketHitGameObject

	printf("No handler for PacketType %d", packetType);
}