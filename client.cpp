#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <iostream>

#include <ConnectionTCP.h>
#include <USocket.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5678"

int main()
{
	Connection::Config config_msg = {
		[](Connection* c, std::string msg, std::vector<Connection*> connections) {
			std::cout << connections.size() << "< size of connections" << std::endl;
			std::cout << "new message : " << msg << " on connection : " << c->GetID() << std::endl;
		},
        [](Connection* c) {
            std::cout << "disconnect : " << std::endl;
        },
        [](Connection* c, int err) {
            std::cout << "error : " << err << std::endl;
        }
	};

	USocket client;

	ConnectionTCP* c = (ConnectionTCP*)client.Connect("127.0.0.1", DEFAULT_PORT, Connection::Type::TCP);
	c->AddConfig(config_msg);

	std::string message;
	for(int i = 0; i < 2; i++)
	{
		std::getline(std::cin, message);
		c->Send(message);
	}

	delete c;

	return 0;
}