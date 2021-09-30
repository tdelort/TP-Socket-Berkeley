#pragma once

#include "Connection.h"
#include "Terminal.h"

#include <thread>
#include <vector>
#include <string>
#include <functional>


class USocket
{
private:
    /**
     * Main function of the networkingThread
     * It receives messages from all connections
     * It also runs the run() method of the Terminal if it exists (meaning we ran Listen())
     */
    void Update();

    std::thread m_networkingThread;
    std::vector<Connection*> m_connections;
    Terminal* m_terminal = nullptr;

public:
    USocket();
    ~USocket();

    void Listen(int port);

    Connection *Connect(std::string addr, int port );
};