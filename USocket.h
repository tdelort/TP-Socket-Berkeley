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

    USocket();
    ~USocket();

    void Listen(char* port, Config config);

    //weak_ptr ici
    Connection* Connect(std::string addr, std::string port);

    void Update();

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
    /*snif*/

    std::thread m_networkingThread;
    std::vector<Connection*> m_connections;
    std::vector<Terminal> m_terminals;
    Config m_config;
};