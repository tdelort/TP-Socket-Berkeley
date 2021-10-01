#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "USocket.h"
#include "ConnectionTCP.h"

USocket::USocket()
{
    //je sais pas
}

USocket::~USocket()
{
    //idem
}

void USocket::Listen(int port)
{
    // attend l'arrivée d'un demande de co
}

Connection* USocket::Connect(std::string addr, int port)
{
    // si la demande de co est acceptée, on crée et renvoie la co
}

void USocket::Update()
{
    // c'est un peu cata ici
    for (auto & terminal : m_terminals) {
        ConnectionTCP conn;
        if (terminal.accept(conn))
        {
            m_connections.push_back(conn);
        }
    }

    // à partir de là j'ai à peu près compris
    fd_set readSet;
    FD_ZERO(&readSet);
    int err = select(0, &readSet, nullptr, nullptr, nullptr);

    for( int i = 0; i < readSet.fd_count; ++i )
    {
        SOCKET s = readSet.fd_array[i];
        // J'ai repris le buffer depuis le cours, ça sera sûrement à updater
        char* buffer = (char*) malloc(sizeof(char)*1024);
        int data = recv(s, buffer, 1024, 0);

        free(buffer);
        // là j'avoue je sais pas quoi faire de ce truc, et pas quoi faire ensuite

    }
}