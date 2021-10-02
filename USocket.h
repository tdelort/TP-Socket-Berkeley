#pragma once

#include <thread>
#include <vector>
#include <string>
#include <functional>

class Terminal;
class Connection;

class USocket
{
public:
    struct Config
    {
        std::function<void(Connection*)> OnConnect;
    };

    USocket(Config config);
    ~USocket();

    Terminal Listen(char* port);

    //weak_ptr ici
    Connection* Connect(std::string addr, int port);

private:
    /**
     * Main function of the networkingThread
     * It receives messages from all connections
     * It also runs the run() method of the Terminal if it exists (meaning we ran Listen())
     * 
     * upd()
     *  for Teminal t in terminals
     *   ConnectionTCP conn;
     *   if(t.accept(conn))
     *     connections.push_back(conn) 
     *  
     *  select(readset)
     *  
     *  for socket in readSet
     *    data = recv
     *    socket.onMessage(...., data, .....)
     */
    void Update();

    std::thread m_networkingThread;
    std::vector<Connection*> m_connections;
    std::vector<Terminal> m_terminals;
    Config m_config;
};