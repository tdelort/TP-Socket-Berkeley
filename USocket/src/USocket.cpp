#include "USocket.h"
#include "Terminal.h"

#include <algorithm>
#include <iostream>
#include <thread>

#ifdef __linux__
	#include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h> 
    #include <arpa/inet.h>
	using socket_t = int;
#elif _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
	using socket_t = SOCKET;
#endif
namespace uqac::network
{
    USocket::USocket()
        : m_connections()
    {
        WSADATA wsaData;
        // Initialize Winsock
        int err = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (err != 0) 
        {
            //printf("WSAStartup failed with error: %d\n", err);
            std::cerr << "WSAStartup failed with error : " << err << std::endl;
            exit(1);
        }

        m_networkingThread = std::thread([this](){
            while(m_threadRunning)
            {
                Update();
            }
        });
    }

    USocket::~USocket()
    {
        m_threadRunning = false;
        m_networkingThread.join();

        for(Connection* c : m_connections)
            delete c;

        WSACleanup();
    }

    void USocket::Listen(char* port, Config config)
    {
        m_config = config;
        struct addrinfo *result = nullptr;
        struct addrinfo hints;
        int err;
        socket_t listenSocket = INVALID_SOCKET;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        err = getaddrinfo(nullptr, port, &hints, &result);
        if ( err != 0 ) 
        {
            std::cerr << "getaddrinfo failed with error : " << err << std::endl;
            WSACleanup();
            exit(1);
        }

        // Create a SOCKET for connecting to server
        listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (listenSocket == INVALID_SOCKET) 
        {
            std::cerr << "socket failed with error : " << WSAGetLastError() << std::endl;
            freeaddrinfo(result);
            WSACleanup();
            exit(1);
        }

        // Setup the TCP listening socket
        /*
            When a socket is created with socket(2), it exists in a name
        space (address family) but has no address assigned to it.  bind()
        assigns the address specified by addr to the socket referred to
        by the file descriptor sockfd
        */
        err = bind( listenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (err == SOCKET_ERROR) 
        {
            std::cerr << "bind failed with error : " << WSAGetLastError() << std::endl;
            freeaddrinfo(result);
            closesocket(listenSocket);
            WSACleanup();
            exit(1);
        }

        freeaddrinfo(result);

        /*
            listen() marks the socket referred to by sockfd as a passive
        socket, that is, as a socket that will be used to accept incoming
        connection requests using accept(2).
        */
        err = listen(listenSocket, SOMAXCONN);
        if (err == SOCKET_ERROR) 
        {
            std::cerr << "listen failed with error : " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            exit(1);
        }

        Terminal t(listenSocket);
        m_terminals.push_back(t);
    }

    Connection* USocket::Connect(std::string addr, std::string port, Connection::Type type)
    {
        int err;
        struct addrinfo* results = nullptr,
            * ptr = nullptr,
            hints;

        socket_t s = INVALID_SOCKET;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;

        err = getaddrinfo("127.0.0.1", port.c_str(), &hints, &results);
        if (err)
        {
            std::cerr << "getaddrinfo failed with error : " << err << std::endl;
            exit(1);
        }

        for (ptr = results; ptr != nullptr; ptr = ptr->ai_next)
        {
            s = socket(AF_INET, SOCK_STREAM, 0);
            err = connect(s, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (err == SOCKET_ERROR)
            {
                closesocket(s);
                s = INVALID_SOCKET;
                continue;
            }
            break;
        }
        freeaddrinfo(results);

        if (s == INVALID_SOCKET)
        {
            std::cerr << "connection failed with error : " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(1);
        }

        ConnectionTCP* res = new ConnectionTCP(s);
        m_connections.push_back(res);
        return res;
    }

    std::vector<Connection*> USocket::GetAllConnections()
    {
        return m_connections;
    }

    void USocket::Update()
    {
        // Handle Connect
        for (auto terminal : m_terminals) {
            ConnectionTCP* conn = terminal.acceptConnections();
            if (conn != nullptr)
            {
                m_connections.push_back(conn);
                m_config.OnConnect(conn);
            }
        }
        
        if(m_connections.size() == 0)
            return;

        // Handle Recv
        fd_set readSet;
        FD_ZERO(&readSet);

        // Ajouter tous les sockets clients dans fd_set
        for(Connection* c : m_connections)
        {
            FD_SET(c->m_s, &readSet);
        }

        struct timeval tv = { 0, 25 };

        int err = select(0, &readSet, nullptr, nullptr, &tv);

        std::transform(m_connections.begin(), m_connections.end(), m_connections.begin(),
            [readSet, this](Connection* c) -> Connection*
            {
                if (FD_ISSET(c->m_s, &readSet))
                {
                    // receive
                    std::string res = "";
                    char recvbuf[ConnectionTCP::RECV_BUF_LENGTH];

                    int ret = recv(c->m_s, recvbuf, ConnectionTCP::RECV_BUF_LENGTH, 0);

                    if (ret > 0)
                    {
                        for (int i = 0; i < ret; i++)
                        {
                            res += recvbuf[i];
                        }

                        c->m_config.OnMessage(c, res, m_connections);
                    }
                    else if (ret == 0)
                    {
                        c->m_config.OnDisconnect(c);
                        delete c;
                        return nullptr;
                    }
                    else
                    {
                        c->m_config.OnError(c, WSAGetLastError());
                        delete c;
                        return nullptr;
                    }
                }
                return c;
            }
        );

        m_connections.erase(std::remove(
            m_connections.begin(),
            m_connections.end(),
            nullptr
        ), m_connections.end());
    }

}