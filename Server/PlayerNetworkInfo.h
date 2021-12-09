#pragma once

#include <Buffer.h>

#include <winsock2.h>

class PlayerNetworkInfo
{
public:
	unsigned short port;
	struct sockaddr_in fromSocket;
};