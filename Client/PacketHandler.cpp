#include "PacketHandler.h"
#include "Client.h"

#include <PacketUpdatePosition.h>

// 0 = PacketUpdatePosition
void PacketHandler::HandlePacket(int packetType, Buffer& buffer, Client* client)
{
	printf("Handling PacketType %d...\n", packetType);
	if (packetType == 0)
	{
		PacketUpdatePosition positionPacket(buffer);
		return;
	}

	printf("No handler for PacketType %d", packetType);
}