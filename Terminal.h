#pragma once

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

#include "Connection.h"

#include <vector>

/**
 * Main job of Terminal is to run in the background and accept() all incoming connections.
 * Accepting a connection will add the new client socket in a container
 * Then, the server will use select() to read/write on all sockets at once. 
 * 
 * Only one new thread per server (we don't want to deal with tens or hundreds of threads)
 */
class Terminal
{
private:
    /* data */
    SOCKET m_listenSocket;

    std::vector<SOCKET> m_clientSockets;
    std::vector<Connection*> m_connexions;

    fd_set m_readingSet;
    fd_set m_writingSet;
    fd_set m_errorSet;

    int m_nfds;

    bool m_continueRunning = true;
public:
    Terminal(SOCKET listenSocket);
    ~Terminal();

    void run();
};