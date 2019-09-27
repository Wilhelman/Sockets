#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#include <iostream>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")

#define LISTEN_PORT 8888

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

void server(int port)
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
	SOCKET s_socket = socket(AF_INET, SOCK_STREAM, 0);

	// TODO-3: Configure socket for address reuse
	int enable = 1;
	int res = setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)& enable, sizeof(int));
	if (res == SOCKET_ERROR) {
		printWSErrorAndExit("Addres reuse");
	}

	// TODO-4: Create an address object with any local address
	struct sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET; // IPv4
	bindAddr.sin_port = htons(port); // Port
	bindAddr.sin_addr.S_un.S_addr = INADDR_ANY; // Any local IP address

	// TODO-5: Bind socket to the local address
	res = bind(s_socket, (const struct sockaddr*) &bindAddr, sizeof(bindAddr));	if (res != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Bind socket to the local address");
	}
	struct sockaddr_in  cli_addr;
	// TODO-6: Make the socket enter into listen mode
	res = listen(s_socket, 10);
	if (res != NO_ERROR)
	{
		// Log and handle error
		printWSErrorAndExit("Listen mode");
	}

	// TODO-7: Accept a new incoming connection from a remote host
	// Note that once a new connection is accepted, we will have
	// a new socket directly connected to the remote host.
	//int address_length = 0;
	int clilen = sizeof(cli_addr);
	SOCKET connection_socket = accept(s_socket, (struct sockaddr*) & cli_addr, &clilen);
	if (connection_socket == INVALID_SOCKET) {
		printWSErrorAndExit("Accept a new incoming connection from a remote host");
	}
	while (true)
	{
		// TODO-8:
		// - Wait a 'ping' packet from the client
		char buffer[1024] = { 0 };
		int res_rec = recv(connection_socket, (char*)&buffer, 1024, 0);
		if (res_rec != SOCKET_ERROR) {
			std::cout << "Received something!" << std::endl;
			std::cout << buffer << std::endl;
		}
		else {
			printWSErrorAndExit("Wait a 'ping' packet from the client");
		}
		// - Send a 'pong' packet to the client
		// - Control errors in both cases
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
	server(LISTEN_PORT);

	PAUSE_AND_EXIT();
}
