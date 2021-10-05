#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "ConnectionTCP.h"
#include "USocket.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5678"

int main()
{
	Connection::Config config_msg = {
		[](Connection* c, std::string msg) {
			std::cout << "new message : " << msg << std::endl;
		}
	};

	USocket client;

	ConnectionTCP* c = (ConnectionTCP*)client.Connect("127.0.0.1", DEFAULT_PORT);

	std::string message;
	while (true)
	{
		//client.Update();

		std::getline(std::cin, message);
		c->Send(message);
	}

	return 0;
}