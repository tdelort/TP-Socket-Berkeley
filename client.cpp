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
#include <span.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5678"

int main()
{
	uqac::network::Connection::Config config_msg = {
		[](uqac::network::Connection* c, uqac::span<char> msg, std::vector<uqac::network::Connection*> connections) {
            for (auto it = msg.begin(); it != msg.end(); ++it)
                std::cout << *it;
            std::cout << std::endl;
		},
        [](uqac::network::Connection* c) {
            std::cout << "disconnect : " << std::endl;
        },
        [](uqac::network::Connection* c, int err) {
            std::cout << "error : " << err << std::endl;
        }
	};

	uqac::network::USocket client;

	uqac::network::ConnectionTCP* c = (uqac::network::ConnectionTCP*)client.Connect("127.0.0.1", DEFAULT_PORT, uqac::network::Connection::Type::TCP);
	c->AddConfig(config_msg);

	std::cout << "Tapez un message pour l'envoyer, ou rien pour quitter" << std::endl;

	std::string message;
	while(true)
	{
		message = "";
		std::getline(std::cin, message);
		if(message.size() < 1)
			break;

		char *tmp = new char[message.size() + 1];
		strcpy(tmp, message.c_str());
		uqac::span<char> msg(tmp, message.size());
		c->Send(msg);
		delete[] tmp;
	}

	delete c;

	return 0;
}