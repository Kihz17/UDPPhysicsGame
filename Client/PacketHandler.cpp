#include "PacketHandler.h"
#include "Client.h"

#include <PacketUpdatePosition.h>

// 0 = PacketUpdatePosition (Client -> Server & Server -> Client)
// 1 = PacketPlayerConnectSuccess (Server -> Client)
// 2 = PacketSpawnGameObject (Server -> Client)
// 3 = PacketDestroyGameObject (Server -> Client)
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Client* client)
{
	printf("Handling PacketType %d...\n", packetType);
	if (packetType == 0)
	{
		PacketUpdatePosition positionPacket(buffer);
		// TODO: Update local copies of game object positions
		return;
	}
	else if (packetType == 1)
	{
		// TODO: Cache our own playerID so that we can validate things like position with the server later on
		return;
	}
	else if (packetType == 2)
	{
		// TODO: Start rendering the gameobject
		return;
	}
	else if (packetType == 3)
	{
		// TODO: Stop rendering the gameobject
		return;
	}

	printf("No handler for PacketType %d", packetType);
}