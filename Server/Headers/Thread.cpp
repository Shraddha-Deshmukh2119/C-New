#include "Thread.h"
#include "Server.h"
#include <iostream>
#include <sstream>

void Thread::create()
{
    free = true;
    destroyed = false;

    CloseThread();
    handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker, &data, CREATE_SUSPENDED, nullptr);
}

bool Thread::available() const
{
    return (free && !destroyed ? true : false);
}

void Thread::enter(SOCKET &Client)
{
    free = false;
    destroyed = false;
    data.client = Client;
    ResumeThread(handle);
}

void Thread::setFree()
{
    free = true;
    destroyed = true;
}

bool Thread::isFree() const
{
    return free;
}

bool Thread::isDestroyed() const
{
    return destroyed;
}

void Thread::CloseThread()
{
    CloseHandle(handle);
    handle = nullptr;
}

HANDLE Thread::getHandle() const
{
    return handle;
}

SOCKET Thread::getClient() const
{
    return data.client;
}

void Thread::endServer()
{
    if (data.server != nullptr)
        data.server->exit = true;
}

#ifdef ONLINE_SHOPPING_UNIT_TEST
void Thread::enableTestMode()
{
    testMode = true;
}

void Thread::pushRecv(const std::string& message)
{
    recvQueue.push_back(message);
}

const std::vector<std::string>& Thread::sentMessages() const
{
    return sendLog;
}

void Thread::clearSentMessages()
{
    sendLog.clear();
}
#endif

void worker(Data *d)
{
    Data &D = *d;
    Server &server = *(D.server);

    if (server.handler != NULL)
        server.handler(server.pool[D.index]);
    else
        std::cerr << " * Unable to pass command to the handler function" << std::endl;

    server.pool[D.index].setFree();
    server.pool[D.index].CloseThread();
}

int Thread::Send(const std::string& str) const
{
#ifdef ONLINE_SHOPPING_UNIT_TEST
    if (testMode)
    {
        sendLog.push_back(str);
        return static_cast<int>(str.size());
    }
#endif
    return send(data.client, str.c_str(), str.size() + 1, 0);
}

int Thread::Send(const int& var) const
{
    std::ostringstream ss;
    ss << var;
    return Send(ss.str());
}

int Thread::Send(const double& var) const
{
    std::ostringstream ss;
    ss << var;
    return Send(ss.str());
}

int Thread::Rec(std::string &str)
{
#ifdef ONLINE_SHOPPING_UNIT_TEST
    if (testMode)
    {
        if (recvQueue.empty())
        {
            str.clear();
            return 0;
        }
        str = recvQueue.front();
        recvQueue.pop_front();
        return static_cast<int>(str.size());
    }
#endif
    char buffer[4069];
    ZeroMemory(buffer, sizeof(buffer));
    
    int bytesRec = recv(data.client, buffer, sizeof(buffer), 0);
    str = std::string(buffer, 0, bytesRec);
    return bytesRec;
}

// extra function;
std::string Thread::getActivity()
{
    return data.server->getActivity();
}
void Thread::closeClient(){
    data.server->closeSocket(data.client);
}