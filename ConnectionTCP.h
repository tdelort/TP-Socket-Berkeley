#pragma once
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Connection.h"

class ConnectionTCP : Connection
{
public:
	void Send(const std::string msg) override;
	void Shutdown() override;
	std::string Receive() override;
	static const int RECV_BUF_LENGTH = 512;
};
