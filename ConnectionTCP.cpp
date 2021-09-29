#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ConnectionTCP.h"

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
    char recvbuf[ConnectionTCP::RECV_BUF_LENGTH];
    int err;
    do {

        err = recv(m_s, recvbuf, 512, 0);
        if (err > 0)
            printf("Bytes received: %d\n", err);
        else if (err == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (err > 0);
}