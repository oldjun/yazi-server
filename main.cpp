#include <iostream>
using namespace std;

#include "Singleton.h"
#include "System.h"
using namespace yazi::utility;

#include "Server.h"
using namespace yazi::server;

int main()
{
    Singleton<System>::instance()->init();

    Server * server = Singleton<Server>::instance();
    server->listen("127.0.0.1", 8080);
    server->start();

    return 0;
}
