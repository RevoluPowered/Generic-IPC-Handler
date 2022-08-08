#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ipc.h"

#ifdef _WIN32
#define SOCKET_NAME "c:/som74yhe.socket"
#else
#define SOCKET_NAME "/tmp/som74yhe.socket"
#endif

#include <thread>

atomic_bool keep_server_on, server_updated;
void ServerThread()
{
    IPCServer server;
    // ensure server starts
    CHECK(server.setup(SOCKET_NAME));

    bool poll_update;
    while( (poll_update = server.poll_update()) && keep_server_on )
    {
        server_updated = true;
        printf("server update\n");
    }
}

TEST_CASE("hello test")
{
    IPCClient client;
    keep_server_on = true;
    server_updated = false;
    // ensure failure
    char hello[] = { "Hello World!\0"};
    //CHECK(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello) ) == false);

    thread server(ServerThread);
    bool first = true;
    while(!server_updated){
        if(first)
        {
            printf("waiting for server\n");
            first=false;
        }
    }

    CHECK(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello) ));
    keep_server_on = false;
    server.join(); // wait for close
}