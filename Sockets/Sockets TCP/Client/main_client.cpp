#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#include <iostream>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_ADDRESS "127.0.0.1"

#define SERVER_PORT 8888

#define PAUSE_AND_EXIT() system("pause"); exit(-1)

void printWSErrorAndExit(const char *msg)
{
	wchar_t *s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&s, 0, NULL);
	fprintf(stderr, "%s: %S\n", msg, s);
	LocalFree(s);
	PAUSE_AND_EXIT();
}

void client(const char *serverAddrStr, int port)
{
	// TODO-1: Winsock init
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Winsock init");
	}

	// TODO-2: Create socket (IPv4, stream, TCP)
	SOCKET c_socket = socket(AF_INET, SOCK_STREAM, 0);

	// TODO-3: Create an address object with the server address
	struct sockaddr_in remoteAddr;
	remoteAddr.sin_family = AF_INET; // IPv4
	remoteAddr.sin_port = htons(port); // Port
	const char* remoteAddrStr = "127.0.0.1"; // Not so remote� :-P
	inet_pton(AF_INET, remoteAddrStr, &remoteAddr.sin_addr);

	// TODO-4: Connect to server
	int res = connect(c_socket, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr));
	if (res != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Connect to server");
	}
	for (int i = 0; i < 5; ++i)
	{
		// TODO-5:
		// - Send a 'ping' packet to the server
		const char* buf = "PING";
		int res_s = send(c_socket, buf, 5, 0);		if (res_s == SOCKET_ERROR) {
			printWSErrorAndExit("Send a 'ping' packet to the server");
		}		std::cout << i << std::endl;
		// - Receive 'pong' packet from the server
		// - Control errors in both cases
		// - Control graceful disconnection from the server (recv receiving 0 bytes)
	}

	// TODO-6: Close socket
	int closesocket(SOCKET s);
	if (closesocket != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Close socket");
	}

	// TODO-7: Winsock shutdown
	iResult = WSACleanup();
	if (iResult != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Winsock shutdown");
	}
}

int main(int argc, char **argv)
{
	Sleep(1000);
	client(SERVER_ADDRESS, SERVER_PORT);

	PAUSE_AND_EXIT();
}
