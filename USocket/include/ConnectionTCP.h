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

class ConnectionTCP : public Connection
{
public:
	ConnectionTCP(socket_t s);
	~ConnectionTCP();
	void Send(const std::string msg) override;
	static const int RECV_BUF_LENGTH = 512;
	Connection::Type ConnectionType() override;
};
