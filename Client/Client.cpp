#include "Client.h"

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

Client::Client()
	: serverSocket(INVALID_SOCKET)
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

	Send(); // TODO: Send start packet

	char ipv4[INET_ADDRSTRLEN];
	inet_ntop(toAddress.sin_family, &toAddress.sin_addr, ipv4, sizeof(ipv4));

	// Our server is ready 
	printf("[CLIENT] Server IP: %s\n", ipv4);
	printf("[CLIENT] Server Port: %d\n", htons(toAddress.sin_port));
	printf("[CLIENT] READY\n");
}

void Client::OnUpdate(float deltaTime)
{
	if (lastPacketTime >= 10000.0f)
	{
		printf("WARNING: We haven't received a packet in 10 seconds, have we lost connection?");
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
	printf("Got data %d", result);
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