#include <iostream>
using namespace std;

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
