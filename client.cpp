#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "ConnectionTCP.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5678"

void Init()
{
	int err;
	WSADATA wsaData;
	// Initialize Winsock
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) 
	{
		printf("WSAStartup failed: %d\n", err);
		exit(1);
	}

}

ConnectionTCP Connect()
{
	int err;
	struct addrinfo *results = NULL,
                    *ptr = NULL,
                    hints;

	SOCKET s = INVALID_SOCKET;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	
	err = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &results);
	if (err)
	{
		printf("getaddrinfo failed: %d\n", err);
		exit(1);
	}
	
	for (ptr = results; ptr != NULL; ptr = ptr->ai_next)
	{
		s = socket(AF_INET, SOCK_STREAM, 0);
		err = connect(s, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (err == SOCKET_ERROR) 
		{
			closesocket(s);
			s = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(results);

	if(s == INVALID_SOCKET)
	{
        printf("connection failed with error: %d\n", WSAGetLastError());
        WSACleanup();
		exit(1);
	}

	return ConnectionTCP(s);
}

void Cleanup()
{
	WSACleanup();
}

int main()
{
	Init();

	ConnectionTCP c = Connect();
	
	// ============== Refactored in the Connection class ================
	c.Send("Hello");

	c.Shutdown();

	//std::string res = c.Receive();

	//std::cout << res << std::endl;
	// ============== End of Connection class ================

	Cleanup();

	return 0;
}
