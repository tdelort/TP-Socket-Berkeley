#pragma once
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Connection.h"

class ConnectionTCP : public Connection
{
public:
	ConnectionTCP(SOCKET s);
	~ConnectionTCP();
	void Send(const std::string msg) override;
	static const int RECV_BUF_LENGTH = 512;
};
