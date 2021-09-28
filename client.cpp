#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

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

SOCKET Connect()
{
	int err;
	struct addrinfo *results = NULL,
                    *ptr = NULL,
                    hints;
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	
	err = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &results);
	if (err)
	{
		printf("getaddrinfo failed: %d\n", err);
		return 1;
	}
	
	for (ptr = results; ptr != NULL; ptr = ptr->ai_next)
	{
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

	return s;
}

void Cleanup(SOCKET s)
{
	int err = closesocket(s);
	WSACleanup();
}

int main()
{
	const char *sendbuf = "Hello";
	char recvbuf[512];
	int err;

	Init();

	SOCKET s = Connect();
	
	// ============== Refactored in the Connection class ================
	err = send(s, sendbuf, (int)strlen(sendbuf), 0 );
    if (err == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

	printf("Bytes Sent: %ld\n", err);

	err = shutdown(s, SD_SEND);
    if (err == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

	do {

        err = recv(s, recvbuf, 512, 0);
        if ( err > 0 )
            printf("Bytes received: %d\n", err);
        else if ( err == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( err > 0 );
	// ============== End of Connection class ================

	Cleanup(s);

	return 0;
}
