#include "ConnectionTCP.h"

#include <string>
#include <iostream>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
    #include <winsock2.h>
	using socket_t = SOCKET;
#endif

namespace uqac::network
{
    ConnectionTCP::ConnectionTCP(socket_t s)
    {
        m_s = s;
    }

    ConnectionTCP::~ConnectionTCP()
    {
        int err = shutdown(m_s, SD_SEND);
        if (err == SOCKET_ERROR) {
            std::cerr << "shutdown failed with error : " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(1);
        }
        closesocket(m_s);
    }

    void ConnectionTCP::Send(const std::string msg)
    {
        const char* sendbuf = msg.c_str();
        int err = send(m_s, sendbuf, (int)strlen(sendbuf), 0);
        if (err == SOCKET_ERROR) {
            std::cerr << "send failed with error : " << WSAGetLastError() << std::endl;
            closesocket(m_s);
            WSACleanup();
            exit(1);
        }
        printf("Bytes Sent: %ld\n", err);
    }

    Connection::Type ConnectionTCP::ConnectionType()
    {
        return Connection::Type::TCP;
    }
}