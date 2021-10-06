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
    : m_listenSocket(listenSocket)
{
}

Terminal::~Terminal()
{
}

ConnectionTCP* Terminal::acceptConnections()
{
    fd_set readingSet;

    FD_ZERO(&readingSet);

    FD_SET(m_listenSocket, &readingSet);

    struct timeval tv = {0, 50};
    int ret = select(m_listenSocket, &readingSet, NULL, NULL, &tv);

    if(ret > 0)
    {
        if(FD_ISSET(m_listenSocket, &readingSet))
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

    return nullptr;
}