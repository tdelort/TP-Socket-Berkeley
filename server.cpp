#include <USocket.h>
#include <Terminal.h>
#include <ConnectionTCP.h>
#include <Connection.h>

#include <string>
#include <iostream>
#include <algorithm>

#define DEFAULT_PORT "5678"

int main(void) 
{
    uqac::network::Connection::Config config_msg = {
        [](uqac::network::Connection* c, std::string msg, std::vector<uqac::network::Connection*> connections) {
			std::cout << c->GetID() << " sent : " << msg << std::endl;
            std::for_each(connections.begin(), connections.end(), [msg, c](uqac::network::Connection* target)
            { 
                if (c->GetID() != target->GetID()) { target->Send(c->GetID() + "> " + msg); };
            });
        },
        [](uqac::network::Connection* c) {
            std::cout << "disconnect : " << c->GetID() << std::endl;
        },
        [](uqac::network::Connection* c, int err) {
            std::cout << "error : " << err << std::endl;
        }
    };

    uqac::network::USocket::Config config_c = {
        [config_msg](uqac::network::Connection* c) {
            std::cout << "new connection : " << c << std::endl;
            c->AddConfig(config_msg);
        }
    };
 
    uqac::network::USocket server;

    server.Listen("5678", config_c);

    std::string dummy;
    std::cin >> dummy; 

    return 0;
}