#include "Terminal.h"
#include "ConnectionTCP.h"

#include <iostream>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
    #include <winsock2.h>
	using socket_t = SOCKET;
#endif

Terminal::Terminal(socket_t listenSocket)
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
    int ret = select((int)m_listenSocket, &readingSet, nullptr, nullptr, &tv);

    if(ret > 0)
    {
        if(FD_ISSET(m_listenSocket, &readingSet))
        {
            // An accept is pending
            std::cout << "accept is pending" << std::endl;
            socket_t clientSocket = accept(m_listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) 
            {
                std::cerr << "accept failed with error : " << WSAGetLastError() << std::endl;
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
        std::cerr << "select failed with error : " << ret << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        exit(1);
    }

    return nullptr;
}