# yazi-server
a c++ tcp server with epoll and thread pool. the demo show an echo server, you can write a general task to accomplish more  complex service.

|Author|junping|
|---|---|
|E-mail|oldjun@sina.com|
|Wechat|chenjunping1024|

## The echo task
task/EchoTask.h
```c++
#pragma once

#include "Task.h"
using namespace yazi::thread;

#include "Socket.h"
using namespace yazi::socket;

namespace yazi {
namespace task {

class EchoTask : public Task
{
public:
    EchoTask(Socket * socket);
    virtual ~EchoTask();

    virtual void run();
    virtual void destroy();
};

}}
```

task/EchoTask.cpp
```c++
#include "EchoTask.h"
#include "Logger.h"
#include "Singleton.h"
#include "SocketHandler.h"

using namespace yazi::utility;
using namespace yazi::socket;
using namespace yazi::thread;
using namespace yazi::task;

EchoTask::EchoTask(Socket * socket) : Task(socket)
{
}

EchoTask::~EchoTask()
{
}

void EchoTask::run()
{
    debug("echo task run");
    SocketHandler * handler = Singleton<SocketHandler>::instance();

    Socket * socket = static_cast<Socket *>(m_data);
    char buf[1024];
    memset(buf, 0, 1024);
    int len = socket->recv(buf, 1024);
    if (len > 0)
    {
        debug("recv msg len: %d msg data: %s", len, buf);
        socket->send(buf, len);
        handler->attach(socket);
    }
    else
    {
        debug("echo task socket closed by peer");
        handler->remove(socket);
    }
}

void EchoTask::destroy()
{
    debug("echo task destory");
    delete this;
}
```
## Define & Register the echo task creator
server/Server.cpp
```c++
Task * echo_task_creator(Socket * socket)
{
    return new EchoTask(socket);
}

// initialize the socket handler
SocketHandler * socket_handler = Singleton<SocketHandler>::instance();

// register the echo task creator
socket_handler->creator(echo_task_creator);
```
## Start the echo server
```c++
#include <iostream>

#include "Singleton.h"
using namespace yazi::utility;

#include "Server.h"
using namespace yazi::server;

int main()
{
    Server * server = Singleton<Server>::instance();
    server->listen("127.0.0.1", 15599);
    server->start();

    return 0;
}
```
## Compile & Run the project
download the source code, cd into the yazi-server project working directory, run command make && ./main 
```bash
cd ./yazi-server
make
./main
```

