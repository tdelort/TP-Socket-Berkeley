#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "ConnectionTCP.h"

ConnectionTCP::ConnectionTCP(SOCKET s)
{
    m_s = s;
}

ConnectionTCP::~ConnectionTCP()
{
    int err = shutdown(m_s, SD_SEND);
    if (err == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
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
        printf("send failed with error: %d\n", WSAGetLastError());
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