#include "server.h"

int main(int argc, const char * argv[])
{
    Server *server = new Server(6566);
    server->start();
    return 0;
}
