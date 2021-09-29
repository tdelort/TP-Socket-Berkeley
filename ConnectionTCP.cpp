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

void ConnectionTCP::Shutdown()
{
    int err = shutdown(m_s, SD_SEND);
    if (err == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_s);
        WSACleanup();
        exit(1);
    }
}

std::string ConnectionTCP::Receive()
{
    std::string res = "";
    char recvbuf[ConnectionTCP::RECV_BUF_LENGTH];
    int err;
    do {

        err = recv(m_s, recvbuf, 512, 0);
        if (err > 0) 
        {
            printf("Bytes received: %d\n", err);
            for (int i = 0; i < err; i++)
            {
                res += recvbuf[i];
            }
        }
        else if (err == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (err > 0);
    return res;
}