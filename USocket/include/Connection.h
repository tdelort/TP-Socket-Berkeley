#pragma once

#include <functional>
#include <string>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "Ws2_32.lib")
	using socket_t = SOCKET;
#endif

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
	socket_t m_s;
	Config m_config;
};