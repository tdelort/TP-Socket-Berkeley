#include "USocket.h"
#include "Terminal.h"
#include "ConnectionTCP.h"

#include <string>
#include <iostream>

int main(void) 
{
    USocket::Config config = {
        [](Connection* c) {
            std::cout << "new connection" << c << std::endl;
        }
    };
 
    USocket server(config);

    server.Listen("5678");
    while (true)
    {
        server.Update();
    }

    return 0;
}