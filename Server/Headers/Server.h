#pragma once

#include <ws2tcpip.h>
#include "Thread.h"
#pragma comment(lib, "ws2_32.lib")
#define CLIENTS (5)     // Maximum number of clients that can be handled at once

using FnPtr = void (*)(Thread &);
using SocketSet = fd_set;

class Server
{
    friend void worker(Data *);
    friend class Thread;

public:
    Server(FnPtr function, int port = 7777) : handler(function), m_port(port) {};
    ~Server();

    bool start();
    void run();

#ifdef ONLINE_SHOPPING_UNIT_TEST
    void requestStop() { exit = true; }
#endif

private:
    WSAData wsaData;
    SocketSet sockets;
    SOCKET server;

    // by default, the server will operate on local host
    const std::string m_ipAddress = "127.0.0.1";
    const unsigned m_port = 7777;
    const FnPtr handler = NULL;
    bool exit = false;  

    Thread pool[CLIENTS];       // Thread pool
    HANDLE handles[CLIENTS];    // Thread Handles

    void Listen();                  // create a listening socket
    void processReq(SocketSet &);   // handels client request
    void createThreads();           // resumes already suspended threads 
    void threadPool(SOCKET &);      // passes the client to thread pool
    
    // extra function;
    void updateActivity(SOCKET&);   // updates the activity file
    std::string getActivity();      // recceive the activity file
    void closeSocket(SOCKET&);
};