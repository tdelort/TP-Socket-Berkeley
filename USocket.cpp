#include "USocket.h"
#include "Terminal.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

USocket::USocket()
{
    WSADATA wsaData;
    // Initialize Winsock
    int err = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (err != 0) 
    {
        printf("WSAStartup failed with error: %d\n", err);
        exit(1);
    }
}

USocket::~USocket()
{
    // need to close sockets too
    WSACleanup();
}

Terminal USocket::Listen(char* port)
{
    struct addrinfo *result = NULL;
    struct addrinfo hints;
	int err;
    SOCKET listenSocket = INVALID_SOCKET;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    err = getaddrinfo(NULL, port, &hints, &result);
    if ( err != 0 ) 
    {
        printf("getaddrinfo failed with error: %d\n", err);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) 
    {
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
    if (err == SOCKET_ERROR) 
    {
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
    if (err == SOCKET_ERROR) 
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    Terminal t = Terminal(listenSocket);
    m_terminals.push_back(t);
    return t;
}

Connection* USocket::Connect(std::string addr, int port)
{
    // si la demande de co est acceptée, on crée et renvoie la co
    return nullptr;
}

void USocket::Update()
{
    // c'est un peu cata ici
    for (auto terminal : m_terminals) {
        ConnectionTCP* conn = terminal.acceptConnections();
        if (conn != nullptr)
            m_connections.push_back(conn);
    }

    // à partir de là j'ai à peu près compris
    fd_set readSet;
    FD_ZERO(&readSet);

    // Ajouter tous les sockets clients dans fd_set
    std::for_each(m_connections.cbegin(), m_connections.cend(), [readSet](Connection* c){FD_SET(c->m_s, &readSet); });

    int err = select(0, &readSet, nullptr, nullptr, nullptr);

    for(Connection* c : m_connections)
    {
        if(FD_ISSET(c->m_s, &readSet))
        {
            // J'ai repris le buffer depuis le cours, ça sera sûrement à updater
            char* buffer = (char*) malloc(sizeof(char)*1024);
            int data = recv(c->m_s, buffer, 1024, 0);

            free(buffer);
            // là j'avoue je sais pas quoi faire de ce truc, et pas quoi faire ensuite

        }
    }
}