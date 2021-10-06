#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "ConnectionUDP.h"

ConnectionUDP::ConnectionUDP(SOCKET s)
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