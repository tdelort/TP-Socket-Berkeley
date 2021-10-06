#pragma once
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Connection.h"

class ConnectionUDP : public Connection
{
public:
    ConnectionUDP(SOCKET s);
    ~ConnectionUDP();
    void Send(const std::string msg) override;
    Connection::Type ConnectionType() override;
};
