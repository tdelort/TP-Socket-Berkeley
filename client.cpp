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

void Cleanup()
{
	WSACleanup();
}

int main()
{
	Connection::Config config_msg = {
		[](Connection* c, std::string msg) {
			std::cout << "new message : " << msg << std::endl;
		}
	};

	USocket client;

	ConnectionTCP* c = (ConnectionTCP*)client.Connect("127.0.0.1", DEFAULT_PORT);
	
	// ============== Refactored in the Connection class ================
	printf("tkt meme pas frero, on va send\n");
	Sleep(1000);
	c->Send("Hello");

	c->Shutdown();

	//std::string res = c.Receive();

	//std::cout << res << std::endl;
	// ============== End of Connection class ================

	Cleanup();

	return 0;
}