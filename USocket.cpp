#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "USocket.cpp"

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
    
}