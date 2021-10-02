#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#include "Terminal.h"
#include "ConnectionTCP.h"

#include <algorithm>
#include <iostream>

Terminal::Terminal(SOCKET listenSocket)
    : m_listenSocket(listenSocket), m_nfds(listenSocket)
{
}

Terminal::~Terminal()
{
}

ConnectionTCP* Terminal::acceptConnections()
{
    FD_ZERO(&m_readingSet);

    FD_SET(m_listenSocket, &m_readingSet);

    struct timeval tv = {0, 50};
    int ret = select(m_nfds, &m_readingSet, NULL, NULL, &tv);

    if(ret > 0)
    {
        if(FD_ISSET(m_listenSocket, &m_readingSet))
        {
            // An accept is pending
            std::cout << "accept is pending" << std::endl;
            SOCKET clientSocket = accept(m_listenSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) 
            {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(m_listenSocket);
                WSACleanup();
                exit(1);
            }
            ConnectionTCP* conn = new ConnectionTCP(clientSocket);
            return conn;
        }
    }
    else if (ret < 0)
    {
        printf("select failed with error: %d\n", ret);
        closesocket(m_listenSocket);
        WSACleanup();
        exit(1);
    }
    printf("aucune connection detectee\n");

    return nullptr;
}