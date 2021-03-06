#include "Client.h"
#include "PacketHandler.h"
#include "Input.h"

#include <PlayerInfo.h>

#include <Packets.h>

#include <ctime>

std::clock_t curr;
std::clock_t prev;

float lastInputUpdate = 0.0f;

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

Client::Client(World* world)
	: world(world),
	serverSocket(INVALID_SOCKET),
	ourId(-1),
	camera(CreateRef<Camera>(windowWidth, windowHeight)),
	playerController(nullptr),
	updatesPerSecond(20.0f),
	connected(false),
	lastConnectionTime(0.0f)
{
	WSAData WSAData;
	int	iResult;

	// Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0) {
		PrintWSAError();
		return;
	}
}

Client::~Client(void)
{
	delete playerController;
	closesocket(serverSocket);
	WSACleanup();
}

void Client::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}
}

void Client::CreateSocket(const std::string& ip, int port)
{
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSocket == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}

	memset((char*)&toAddress, 0, sizeof(toAddress));
	toAddress.sin_family = AF_INET;
	toAddress.sin_port = htons(port);
	inet_pton(toAddress.sin_family, ip.c_str(), &toAddress.sin_addr.s_addr);

	SetNonBlocking(serverSocket);

	// Send some random data on "connection" to avoid 10022 error
	Buffer buffer(4);
	buffer.WriteUInt32BE(99999999);
	Send(buffer);

	char ipv4[INET_ADDRSTRLEN];
	inet_ntop(toAddress.sin_family, &toAddress.sin_addr, ipv4, sizeof(ipv4));

	// Our server is ready 
	printf("[CLIENT] Server IP: %s\n", ipv4);
	printf("[CLIENT] Server Port: %d\n", htons(toAddress.sin_port));
	printf("[CLIENT] READY\n");
	prev = std::clock();
}

void Client::OnUpdate(float deltaTime)
{
	if (connected && lastPacketTime >= 10.0f)
	{
		printf("WARNING: We haven't received a packet in 10 seconds, have we lost connection?\n");
		connected = false;
	}
	
	if (Input::IsKeyPressed(Key::Enter))
	{
		printf("Sending ready status...\n");
		PacketReadyUp packet;
		Send(packet);
	}
	
	if (!connected)
	{
		lastConnectionTime += deltaTime;
		if (lastConnectionTime >= 10.0f)
		{
			lastConnectionTime = 0.0f;
			printf("Not connected, retrying...\n");
			Buffer buffer(4);
			buffer.WriteUInt32BE(99999999);
			Send(buffer);
		}
	}

	if (playerController)
	{
		if (Input::IsKeyPressed(Key::Escape))
		{
			playerController->ToggleHandleInput(false);
		}
		else if (Input::IsKeyPressed(Key::Tab))
		{
			playerController->ToggleHandleInput(true);
		}

		if (Input::IsKeyPressed(Key::Equal))
		{
			printf("Enabling client side prediction\n");
			playerController->predictionEnabled = true;
		}
		else if (Input::IsKeyPressed(Key::Minus))
		{
			printf("Disabling client side prediction\n");
			playerController->predictionEnabled = false;
		}

		if (Input::IsKeyPressed(Key::Zero))
		{
			printf("Enabling dead reckoning\n");
			world->enableDeadReckoning = true;
		}
		else if (Input::IsKeyPressed(Key::Nine))
		{
			printf("Disabling dead reckoning\n");
			world->enableDeadReckoning = false;
		}

		if (Input::IsKeyPressed(Key::Eight))
		{
			printf("Enabling lerp\n");
			world->enablingLerping = true;
		}
		else if (Input::IsKeyPressed(Key::Seven))
		{
			printf("Disabling lerp\n");
			world->enablingLerping = false;
		}

		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && playerController->lastHit >= 1.0f) // Try to hit
		{
			glm::vec3 rayOrigin = camera->position;
			glm::vec3 ray = glm::normalize(camera->direction);

			// Check if we are looking at any players
			for (ClientGameObject* obj : world->GetGameObjects())
			{
				if (obj->GetId() == ourId) continue;

				glm::vec3 center = glm::vec3(obj->GetTransform()[3]);
				glm::vec3 positionDiff = glm::vec3(playerController->GetTransformSafe()[3]) - center;
				if (glm::dot(positionDiff, positionDiff) > PlayerInfo::MaxHitDistance) continue; // Too far to hit this person

				float radius = obj->radius;

				glm::vec3 difference = rayOrigin - center;
				float a = glm::dot(ray, ray);
				float b = 2.0f * glm::dot(difference, ray);
				float c = glm::dot(difference, difference) - radius * radius;
				float discriminant = b * b - 4 * a * c;
				if (discriminant > 0.0f) // We hit something
				{
					PacketGameObjectHit packet(obj->GetId(), ray);
					Send(packet);
					break;
				}
			}
		}
	}

	curr = std::clock();
	lastInputUpdate = (curr - prev) / double(CLOCKS_PER_SEC);
	if (playerController)
	{
		playerController->UpdateController(deltaTime);

		if (lastInputUpdate >= (1.0f / updatesPerSecond))
		{
			prev = curr;

			int moveStateId = playerController->NextMoveRequestId();
			glm::vec3 position = glm::vec3(playerController->GetTransform()[3]);

			ClientMoveState moveState;
			moveState.postion = position;
			playerController->InsertMoveState(moveStateId, moveState);

			PacketPlayerInput packet(ourId, moveStateId, position);
			Send(packet);
		}
	}	

	struct sockaddr_in from;
	int fromlen = sizeof(from);
	Buffer buffer(512);

	int length = buffer.Length();
	int result = recvfrom(serverSocket, (char*) &buffer.GetData()[0], buffer.Length(), 0, (struct sockaddr*)&from, &fromlen);

	lastPacketTime += deltaTime;

	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return;
		}
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our buffer, and just
		// ignore this.
		buffer.Reset();
		return;
	}

	lastPacketTime = 0.0f;
	int packetType = buffer.ReadUInt32LE();
	PacketHandler::HandlePacket(packetType, buffer, this);
}

void Client::Send(Packet& packet)
{
	Buffer buffer = packet.Serialize();
	Send(buffer);
}

void Client::Send(Buffer& buffer)
{
	int result = sendto(serverSocket, (char*)&buffer.GetData()[0], buffer.Length(), 0, (struct sockaddr*)&toAddress, sizeof(toAddress));

	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
		PrintWSAError();
		return;
	}

	if (result == 0) {
		printf("Disconnected...\n");
		return;
	}
}