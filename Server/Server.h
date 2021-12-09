#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Player.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <Packet.h>

#include <vector>
#include <unordered_map>

#pragma comment(lib, "Ws2_32.lib")

void _PrintWSAError(const char* file, int line);
#define PrintWSAError() _PrintWSAError(__FILE__, __LINE__)

class Server
{
public:
	Server();
	~Server();

	void OnUpdate();

	GameObject* GetGameObject(int id);
	void FlagDirty(GameObject* gameObject, int dirtyIndex);

	double deltaTime;
private:
	void SetNonBlocking(SOCKET socket);
	void ReadData();
	Player* GetPlayer(unsigned short port, struct sockaddr_in fromSocket);
	void BroadcastUpdate();

	void SendTo(Player* player, const Packet& packet);
	void SendTo(Player* player, Buffer& buffer);
	void BroadcastPacket(const Packet& packet);
	void BroadcastPacketExclude(Player* player, const Packet& packet);

	glm::vec3 GetRandomSpawnPoint();

	bool isRunning;

	timeval timeVal;
	SOCKET acceptSocket;
	SOCKET listenSocket;

	std::unordered_map<unsigned short, Player*> players;

	std::unordered_map<int, GameObject*> gameObjects;
	std::unordered_map<int, DirtyGameObject> dirtyGameObjects; // Contains dirty data that a game object needs to update
};