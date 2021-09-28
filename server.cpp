#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void Init()
{
	// If wsaData is useful, make it a class attribute
    WSADATA wsaData;
    // Initialize Winsock
    int err = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        exit(1);
    }
}

SOCKET Listen()
{
    struct addrinfo *result = NULL;
    struct addrinfo hints;
	int err;
    SOCKET listenSocket = INVALID_SOCKET;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    err = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( err != 0 ) {
        printf("getaddrinfo failed with error: %d\n", err);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
	/*
	 	When a socket is created with socket(2), it exists in a name
       space (address family) but has no address assigned to it.  bind()
       assigns the address specified by addr to the socket referred to
       by the file descriptor sockfd
	*/
    err = bind( listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (err == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    freeaddrinfo(result);

	/*
		listen() marks the socket referred to by sockfd as a passive
       socket, that is, as a socket that will be used to accept incoming
       connection requests using accept(2).
	*/
    err = listen(listenSocket, SOMAXCONN);
    if (err == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

	return listenSocket;
}

void Cleanup(SOCKET listenSocket, SOCKET clientSocket)
{
	int err;
    // No longer need server socket
    closesocket(listenSocket);

    // shutdown the connection since we're done
    err = shutdown(clientSocket, SD_SEND);
    if (err == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    // cleanup
    closesocket(clientSocket);
    WSACleanup();
}

int __cdecl main(void) 
{
    int iResult;

    SOCKET clientSocket = INVALID_SOCKET;
    SOCKET listenSocket = INVALID_SOCKET;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
	
	Init();

	listenSocket = Listen();

	// ========================= In the TERMINAL Class ========================
	// This part can be run in a separate thread i think
    // Accept a client socket
	/*
		The accept() system call is used with connection-based socket
       types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
       connection request on the queue of pending connections for the
       listening socket, sockfd, creates a new connected socket, and
       returns a new file descriptor referring to that socket.  The
       newly created socket is not in the listening state.  The original
       socket sockfd is unaffected by this call.
	   */
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
            iSendResult = send( clientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);
	// ======================= End of the TERMINAL Class ======================

	Cleanup(listenSocket, clientSocket);

    return 0;
}