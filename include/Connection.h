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
private:
	static inline unsigned int newID = 0;

public:
	struct Config
	{
		std::function<void(Connection*, std::string, std::vector<Connection*>)> OnMessage;
		std::function<void(Connection*)> OnDisconnect;
		std::function<void(Connection*, int)> OnError;
	};

	Connection();

	virtual void Send(const std::string msg) = 0;
	void AddConfig(Config config);

	unsigned int GetID();

	friend class USocket;

	enum class Type {TCP, UDP};
	virtual Type ConnectionType() = 0;

protected:
	const unsigned int m_id;
	SOCKET m_s;
	Config m_config;
};