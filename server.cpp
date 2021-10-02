#include "USocket.h"
#include "Terminal.h"
#include "ConnectionTCP.h"
#include "Connection.h"

#include <string>
#include <iostream>

int main(void) 
{
    Connection::Config config_msg = {
        [](Connection* c, std::string msg) {
            std::cout << "new message : " << msg << std::endl;
        }
    };

    USocket::Config config_c = {
        [config_msg](Connection* c) {
            std::cout << "new connection : " << c << std::endl;
            c->AddConfig(config_msg);
        }
    };
 
    USocket server(config_c);

    server.Listen("5678");
    while (true)
    {
        server.Update();
    }

    return 0;
}