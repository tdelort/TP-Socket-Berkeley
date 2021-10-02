#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <functional>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class Connection
{
public:
	struct Config
	{
		std::function<void(Connection*, std::string)> OnMessage;
	};

	virtual void Send(const std::string msg) = 0;
	virtual void Shutdown() = 0;
	void AddConfig(Config config);

	friend class USocket;
protected:
	SOCKET m_s;
	Config m_config;
};