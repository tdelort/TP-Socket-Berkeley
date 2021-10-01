#include "USocket.h"
#include "Terminal.h"
#include "ConnectionTCP.h"

#include <string>
#include <iostream>

int main(void) 
{
 
    USocket server;

    Terminal t = server.Listen("5678");
    std::cout << "listen" << std::endl;
    ConnectionTCP* c = nullptr;
    do
    {
        c = t.acceptConnections();
    } while (!c);

    std::string msg = c->Receive();
    std::cout << "msg : " << msg << std::endl;
    c->Send(msg);

    return 0;
}