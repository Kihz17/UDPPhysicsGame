#pragma once

#include <Buffer.h>

class Client;
class PacketHandler
{
public:
	static void HandlePacket(int packetType, Buffer& buffer, Client* client);
};