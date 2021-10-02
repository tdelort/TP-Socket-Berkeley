#pragma once
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

class Connection
{
public:
	virtual void Send(const std::string msg) = 0;
	virtual void Shutdown() = 0;

	friend class USocket;
protected:
	SOCKET m_s;
};