#include "Terminal.h"

#include <algorithm>

Terminal::Terminal(SOCKET listenSocket)
    : m_listenSocket(listenSocket), m_nfds(listenSocket)
{
}

Terminal::~Terminal()
{
}

// struct AddToSet
// {
//     fd_set* m_set;
//     AddToSet(fd_set* set) : m_set(set) {}
//     void operator()(SOCKET s) { FD_SET(s, m_set); }
// };

void Terminal::run()
{
    FD_ZERO(&m_readingSet);
    // FD_ZERO(&m_writingSet);
    // FD_ZERO(&m_errorSet);

    FD_SET(m_listenSocket, &m_readingSet);

    std::for_each(m_clientSockets.cbegin(), m_clientSockets.cend(), [this](SOCKET s){ FD_SET(s, &m_readingSet); });
    // equivalent UnaryFunction
    // std::for_each(m_clientSockets.begin(), m_clientSockets.end(), AddToSet(&m_readingSet));

    int ret = select(m_nfds, &m_readingSet, &m_writingSet, &m_errorSet, 0);

    if(ret > 0)
    {
        if(FD_ISSET(m_listenSocket, &m_readingSet))
        {
            // An accept is pending
        }

        // Not sure it belongs here now
        // std::for_each(m_clientSockets.cbegin(), m_clientSockets.cend(),
        //     [](SOCKET s){
        //          if(FD_ISSET(s, &m_readingSet))
        //          {
        //              // handle recv
        //          }
        //     }
        // );
    }
    else if(ret == 0)
    {

    }
    else
    {

    }
}