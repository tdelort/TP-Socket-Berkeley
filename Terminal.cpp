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

#include "Terminal.h"
#include "ConnectionTCP.h"

#include <algorithm>

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

    int ret = select(m_nfds, &m_readingSet, NULL, NULL, 0);

    if(ret > 0)
    {
        if(FD_ISSET(m_listenSocket, &m_readingSet))
        {
            // An accept is pending
            SOCKET clientSocket = accept(m_listenSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) 
            {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(m_listenSocket);
                WSACleanup();
                exit(1);
            }
            ConnectionTCP* c = new ConnectionTCP(clientSocket);
            return c;
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