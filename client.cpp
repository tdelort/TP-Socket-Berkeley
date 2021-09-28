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
	const char *sendbuf = "Hello";
	char recvbuf[512];

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

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
	

	iResult = send(s, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

	printf("Bytes Sent: %ld\n", iResult);

	iResult = shutdown(s, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

	do {

        iResult = recv(s, recvbuf, 512, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

	// TODO : shutdown
	int err = closesocket(s);
	WSACleanup();

	return 0;
}
