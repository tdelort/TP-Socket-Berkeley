#include <USocket.h>
#include <Terminal.h>
#include <ConnectionTCP.h>
#include <Connection.h>

#include <string>
#include <iostream>
#include <algorithm>

int main(void) 
{
    Connection::Config config_msg = {
        [](Connection* c, std::string msg, std::vector<Connection*> connections) {
			std::cout << "new message : " << msg << " on connection : " << c->GetID() << std::endl;
            std::for_each(connections.begin(), connections.end(), [msg, c](Connection* target)
            { 
                if (c->GetID() != target->GetID()) { target->Send(c->GetID() + "> " + msg); };
            });
			std::cout << "broadcast done" << std::endl;
        },
        [](Connection* c) {
            std::cout << "disconnect : " << std::endl;
        },
        [](Connection* c, int err) {
            std::cout << "error : " << err << std::endl;
        }
    };

    USocket::Config config_c = {
        [config_msg](Connection* c) {
            std::cout << "new connection : " << c << std::endl;
            c->AddConfig(config_msg);
        }
    };
 
    USocket server;

    server.Listen("5678", config_c);

    std::string dummy;
    std::cin >> dummy; 

    return 0;
}