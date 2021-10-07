#pragma once

#include "ConnectionTCP.h"

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
	#include <winsock2.h>
	using socket_t = SOCKET;
#endif

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
    socket_t m_listenSocket;
public:
    Terminal(socket_t listenSocket);
    ~Terminal();

    ConnectionTCP* acceptConnections();
};