#pragma once

#include "Connection.h"

#include <string>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
	#include <winsock2.h>
	using socket_t = SOCKET;
#endif

class ConnectionUDP : public Connection
{
public:
    ConnectionUDP(socket_t s);
    ~ConnectionUDP();
    void Send(const std::string msg) override;
    Connection::Type ConnectionType() override;
};
