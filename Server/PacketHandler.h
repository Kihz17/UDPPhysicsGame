#pragma once

#include "Player.h"
#include <Buffer.h>

class Server;
class PacketHandler
{
public:
	static void HandlePacket(int packetType, Buffer& buffer, Server* server, Player* player);
};