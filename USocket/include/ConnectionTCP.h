#pragma once

#include "Connection.h"
#include "span.h"

#include <string>

#ifdef __linux__
	#include <sys/socket.h>
	using socket_t = int;
#elif _WIN32
	#include <winsock2.h>
	using socket_t = SOCKET;
#endif

namespace uqac::network
{
	class ConnectionTCP : public Connection
	{
	public:
		ConnectionTCP(socket_t s);
		~ConnectionTCP();
		void Send(const span<char> msg) override;
		static const int RECV_BUF_LENGTH = 512;
		Connection::Type ConnectionType() override;
	};
}
