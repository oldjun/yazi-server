#include "Server.h"
using namespace yazi::server;

#include <sys/resource.h>

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "TaskDispatcher.h"
using namespace yazi::thread;

#include "SocketHandler.h"
using namespace yazi::socket;

#include "EchoTask.h"
using namespace yazi::task;

Task * echo_task_creator(Socket * socket)
{
    return new EchoTask(socket);
}

Server::Server()
{
    core_dump();

    m_root_path = get_root_path();

    // initialize the global logger
    Logger::instance()->open(m_root_path + "/main.log");
}

Server::~Server()
{
}

void Server::listen(const string & ip, int port)
{
    // initialize the socket handler
    SocketHandler * socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(ip, port);
}

void Server::start(int threads, int connects)
{
    // initialize the thread pool and task queue
    TaskDispatcher * dispatcher = Singleton<TaskDispatcher>::instance();
    dispatcher->init(threads);

    SocketHandler * socket_handler = Singleton<SocketHandler>::instance();

    // register the echo task creator
    socket_handler->creator(echo_task_creator);
    socket_handler->handle(connects);
}

void Server::core_dump()
{
    // core dump
    struct rlimit x;
    int ret = getrlimit(RLIMIT_CORE, &x);
    x.rlim_cur = x.rlim_max;
    ret = setrlimit(RLIMIT_CORE, &x);
    ret = getrlimit(RLIMIT_DATA, &x);
    x.rlim_cur = 768000000;
    ret = setrlimit(RLIMIT_DATA, &x);
}

string Server::get_root_path()
{
    char path[1024];
    memset(path, 0, 1024);
    int cnt = readlink("/proc/self/exe", path, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        return "";
    }
    for (int i = cnt; i >= 0; --i)
    {
        if (path[i] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}