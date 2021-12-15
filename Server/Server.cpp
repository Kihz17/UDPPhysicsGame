#include "Server.h"
#include "PacketHandler.h"

#include <Buffer.h>
#include <Packets.h>
#include <Utilities.h>

#include <WS2tcpip.h>

#include <ctime>

std::clock_t curr;
std::clock_t prev;

static int gameObjectId = 0;

const glm::vec3 spawnPoints[5] =
{
	{0.0f, 3.0f, 0.0f},
	{0.0f, 3.0f, 0.0f},
	{0.0f, 3.0f, 0.0f},
	{0.0f, 3.0f, 0.0f},
	{0.0f, 3.0f, 0.0f},
};

void _PrintWSAError(const char* file, int line)
{
	int WSAErrorCode = WSAGetLastError();
	wchar_t* s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&s, 0, NULL);
	fprintf(stderr, "[WSAError:%d] %S\n", WSAErrorCode, s);
	LocalFree(s);
}

Server::Server()
	: isRunning(false),
	listenSocket(INVALID_SOCKET),
	acceptSocket(INVALID_SOCKET),
	updatesPerSecond(20.0f)
{
	WSAData WSAData;
	int	iResult;
	int	Port = 5149;
	SOCKADDR_IN ReceiverAddr;

	// Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0) {
		PrintWSAError();
		return;
	}

	// Create a socket
	listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSocket == INVALID_SOCKET) {
		PrintWSAError();
		return;
	}

	// Bind our socket
	ReceiverAddr.sin_family = AF_INET;
	ReceiverAddr.sin_port = htons(Port);
	ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	iResult = bind(listenSocket, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
	if (iResult == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}

	// Set our socket to be nonblocking
	SetNonBlocking(listenSocket);

	char ipv4[INET_ADDRSTRLEN];
	inet_ntop(ReceiverAddr.sin_family, &ReceiverAddr.sin_addr, ipv4, sizeof(ipv4));

	// Our server is ready 
	printf("[SERVER] Receiving IP: %s\n", ipv4);
	printf("[SERVER] Receiving Port: %d\n", htons(ReceiverAddr.sin_port));
	printf("[SERVER] Ready to receive a datagram...\n");

	isRunning = true;
	prev = std::clock();
}

Server::~Server()	
{
	closesocket(listenSocket);
	WSACleanup();	// <-- Not necessary if quitting application, Windows will handle this.
}

void Server::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}
}

void Server::OnUpdate()
{
	if (!isRunning) return;

	ReadData();

	curr = std::clock();
	deltaTime = (curr - prev) / double(CLOCKS_PER_SEC);

	if (deltaTime < (1.0f / updatesPerSecond)) return;
	prev = curr;

	BroadcastUpdate();
	dirtyGameObjects.clear();
}

void Server::ReadData()
{
	struct sockaddr_in from;
	int fromlen = sizeof(from);

	Buffer buffer;

	int result = recvfrom(listenSocket, (char*)&buffer.GetData()[0], buffer.Length(), 0, (struct sockaddr*)&from, &fromlen);
	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our buffer, and just
		// ignore this.
		buffer.Reset();
		return;
	}

	unsigned short port = from.sin_port;
	Player* player = GetPlayer(port, from);

	int packetType = buffer.ReadUInt32LE();
	if (packetType == 99999999) return; // This is a garbage packet that is sent on client inital connection to stop WSA10022 error
	PacketHandler::HandlePacket(packetType, buffer, this); // TODO: Ensure that a client isn't faking their GameObject ID
}

Player* Server::GetPlayer(unsigned short port, struct sockaddr_in fromSocket)
{
	std::unordered_map<unsigned short, Player*>::iterator it = players.find(port);
	if (it != players.end())
	{
		return it->second;
	}

	// Player doesn't exist, make a new one
	glm::vec3 spawnPoint = GetRandomSpawnPoint();
	glm::mat4 playerTransform(1.0f);
	playerTransform[3] = glm::vec4(spawnPoint, 1.0f);
	Player* player = new Player(GetNextGameObjectId(), port, fromSocket, playerTransform);

	players.insert({ port, player });
	gameObjects.insert({ player->GetId(), player});

	// Tell new player we have successfully connected
	PacketPlayerConnectSuccess connectPacket(player->GetId());
	SendTo(player, connectPacket);

	// Show players that a new client has joined
	PacketSpawnGameObject spawnPacket(player->GetId(), player->GetType(), spawnPoint.x, spawnPoint.y, spawnPoint.z);
	BroadcastPacket(spawnPacket);

	// Tell new player about all game objects that currently exist
	{
		std::unordered_map<int, GameObject*>::iterator it = gameObjects.begin();
		while (it != gameObjects.end())
		{
			GameObject* obj = it->second;
			it++;

			if (obj->GetId() == player->GetId()) continue;
			glm::mat4& transform = obj->GetTransform();
			PacketSpawnGameObject spawnPacket(obj->GetId(), obj->GetType(), transform[3].x, transform[3].y, transform[3].z);
			SendTo(player, spawnPacket);
		}
	}

	return player;
}

GameObject* Server::GetGameObject(int id)
{
	std::unordered_map<int, GameObject*>::iterator it = gameObjects.find(id);
	if (it == gameObjects.end())
	{
		printf("GameObject with ID %d not found\n", id);
		return nullptr;
	}

	return it->second;
}

void Server::BroadcastUpdate()
{
	// Grab packets that are needed to update game objects' state
	std::vector<Packet*> updatePackets; // We need to hold a vector of pointers because the class "Packet" is pure virtual

	// Add move state update packet
	{
		std::vector<GameObjectMoveUpdate> updates;
		std::unordered_map<int, GameObject*>::iterator it = gameObjects.begin();
		while (it != gameObjects.end())
		{
			GameObjectMoveUpdate update;
			update.gameObjectId = it->second->GetId();
			update.requestId = it->second->lastMoveRequestId;
			update.position = glm::vec3(it->second->GetTransform()[3]);
			update.velocity = it->second->GetVelocity();
			updates.push_back(update);
			it++;
		}

		updatePackets.push_back(new PacketUpdateGameObjectPositions(updates));
	}

	// See if we have any "dirty" data that needs to be updated
	{
		std::unordered_map<int, DirtyGameObject>::iterator it = dirtyGameObjects.begin();
		while (it != dirtyGameObjects.end())
		{
			std::unordered_map<int, GameObject*>::iterator gameObjectIt = gameObjects.find(it->first);
			if (gameObjectIt == gameObjects.end()) continue; // This object doesn't exist anymore

			GameObject* gameObject = gameObjectIt->second;
			DirtyGameObject& dirtyObject = it->second;

			// Go through dirty indices and make their respective packets
			for (const int& index : dirtyObject.dirtyIndices)
			{
				//if (index == 0) // Position
				//{
				//	glm::mat4& transform = gameObject->GetTransform();
				//	updatePackets.push_back(new PacketUpdatePosition(it->first, transform[3].x, transform[3].y, transform[3].z));
				//}
			}
			it++;
		}
	}

	// Send state updates to all players
	std::unordered_map<unsigned short, Player*>::iterator it = players.begin();
	while (it != players.end())
	{
		for (const Packet* packet : updatePackets)
		{
			SendTo(it->second, *packet); // TODO: Don't keep recreating buffer objects to reduce overhead
		}
		it++;
	}

	for (Packet* packet : updatePackets)
	{
		delete packet;
	}
}

void Server::SendTo(Player* player, const Packet& packet)
{
	Buffer buffer = packet.Serialize();
	int result = sendto(listenSocket, (char*) &buffer.GetData()[0], buffer.Length(), 0, (struct sockaddr*)&(player->networkInfo.fromSocket), sizeof(player->networkInfo.fromSocket));
}

void Server::SendTo(Player* player, Buffer& buffer)
{
	int result = sendto(listenSocket, (char*)&buffer.GetData()[0], buffer.Length(), 0, (struct sockaddr*)&(player->networkInfo.fromSocket), sizeof(player->networkInfo.fromSocket));
}

void Server::BroadcastPacket(const Packet& packet)
{
	Buffer buffer = packet.Serialize();
	std::unordered_map<unsigned short, Player*>::iterator it = players.begin();
	while (it != players.end())
	{
		SendTo(it->second, buffer); 
		it++;
	}
}

void Server::BroadcastPacketExclude(Player* player, const Packet& packet)
{
	Buffer buffer = packet.Serialize();
	std::unordered_map<unsigned short, Player*>::iterator it = players.begin();
	while (it != players.end())
	{
		Player* p = it->second;
		if (p != player)
		{
			SendTo(p, buffer);
		}

		it++;
	}
}

void Server::FlagDirty(GameObject* gameObject, int dirtyIndex)
{
	int id = gameObject->GetId();
	std::unordered_map<int, DirtyGameObject>::iterator it = dirtyGameObjects.find(id);
	if (it == dirtyGameObjects.end()) // Create new dirty entry
	{
		DirtyGameObject dirtyObject;
		dirtyObject.id = id;
		dirtyObject.dirtyIndices.insert(dirtyIndex);
		dirtyGameObjects.insert({ id, dirtyObject });
	}
	else
	{
		DirtyGameObject& dirtyObject = it->second;
		dirtyObject.dirtyIndices.insert(dirtyIndex);
	}
}

glm::vec3 Server::GetRandomSpawnPoint()
{
	return spawnPoints[Utils::GetRandomInt(0, 4)];
}

int Server::GetNextGameObjectId()
{
	return gameObjectId++;
}

void Server::OnShutdown()
{
	printf("Server shutting down...\n");
	PacketServerShutdown packet;
	BroadcastPacket(packet);
}