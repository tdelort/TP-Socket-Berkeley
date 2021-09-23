#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

int main()
{
	WSADATA wsaData;
	int iResult;
	struct addrinfo *results = NULL,
                    *ptr = NULL,
                    hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	
	int s = socket(AF_INET, SOCK_STREAM, 0);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &results);
	if (iResult)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		return 1;
	}
	
	for (ptr = results; ptr != NULL; ptr = ptr->ai_next)
	{
		iResult = connect(s, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(s);
			s = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(results);

	// TODO : shutdown
	int err = closesocket(s);

	return 0;
}
