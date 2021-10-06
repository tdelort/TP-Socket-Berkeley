#pragma once

#include <thread>
#include <vector>
#include <string>
#include <functional>

#include "Connection.h"

class Terminal;

class USocket
{
public:
    struct Config
    {
        std::function<void(Connection*)> OnConnect;
    };

    USocket();
    ~USocket();

    void Listen(char* port, Config config);

    //weak_ptr ici
    Connection* Connect(std::string addr, std::string port, Connection::Type type);

    //std::vector<std::weak_ptr<Connection>> GetAllConnections();
    std::vector<Connection*> GetAllConnections();

private:
    void Update();

    std::thread m_networkingThread;
    bool m_threadRunning;
    std::vector<Connection*> m_connections;
    std::vector<Terminal> m_terminals;
    Config m_config;
};