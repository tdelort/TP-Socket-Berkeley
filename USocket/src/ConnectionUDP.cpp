#include "ConnectionUDP.h"

#include <string>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
    #include <winsock2.h>
	using socket_t = SOCKET;
#endif

ConnectionUDP::ConnectionUDP(socket_t s)
{
    m_s = s;
}

ConnectionUDP::~ConnectionUDP()
{
    int err = shutdown(m_s, SD_SEND);
    if (err == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    closesocket(m_s);
}

void ConnectionUDP::Send(const std::string msg)
{

}

Connection::Type ConnectionUDP::ConnectionType()
{
    return Connection::Type::UDP;
}