#include "ConnectionTCP.h"

#include <string>
#include <iostream>

#include "span.h"

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

    void ConnectionTCP::Send(const span<char> msg)
    {
        int size = msg.size();
        const char* sendbuf = msg.begin();
        int err = send(m_s, sendbuf, size, 0);
        if (err == SOCKET_ERROR) {
            std::cerr << "send failed with error : " << WSAGetLastError() << std::endl;
            closesocket(m_s);
            WSACleanup();
            exit(1);
        }
    }

    Connection::Type ConnectionTCP::ConnectionType()
    {
        return Connection::Type::TCP;
    }
}