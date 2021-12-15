#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NOMINMAX

#include "pch.h"
#include "World.h"
#include "PlayerController.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#include <Buffer.h>
#include <Packet.h>

#include <vector>
#include <string>

void _PrintWSAError(const char* file, int line);
#define PrintWSAError() _PrintWSAError(__FILE__, __LINE__)

class Client
{
public:
	Client(World* world);
	~Client();

	void CreateSocket(const std::string& ip, int port);

	void OnUpdate(float deltaTime);
	inline void UpdateFrequency(float frequency) { updatesPerSecond = frequency; };

	void Send(Packet& packet);
	void Send(Buffer& buffer);

	inline const Ref<Camera> GetCamera() const { return camera; }
	inline PlayerController* GetPlayerController() { return playerController; }

	const float windowWidth = 1700;
	const float windowHeight = 800;

private:
	friend class PacketHandler;

	void SetNonBlocking(SOCKET socket);

	SOCKET serverSocket;
	struct sockaddr_in toAddress;

	FD_SET readSet;

	float lastPacketTime;

	int ourId; // Holds our game object id

	World* world;

	PlayerController* playerController;
	Ref<Camera> camera;

	float updatesPerSecond;
};