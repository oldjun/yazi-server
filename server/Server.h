#pragma once

#include <string>
using namespace std;

namespace yazi {
namespace server {

class Server
{
public:
    Server();
    ~Server();

    void listen(const string & ip, int port);
    void start(int threads = 1024, int connects = 1024);

private:
    void core_dump();
    string get_root_path();

private:
    string m_root_path;
};

}}
