#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

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
	Client();
	~Client();

	void CreateSocket(const std::string& ip, int port);

	void OnUpdate(float deltaTime);

	void Send(Packet& packet);
	void Send(Buffer& buffer);

private:
	void SetNonBlocking(SOCKET socket);

	SOCKET serverSocket;
	struct sockaddr_in toAddress;

	FD_SET readSet;

	float lastPacketTime;
};