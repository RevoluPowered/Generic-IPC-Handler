#include <iostream>
#include "ipc.h"

int main() {

    IPCServer server;
    IPCClient client;

    server.setup();
    client.setup();

    while(true)
    {
        server.poll();
        client.poll();
    }
    return 0;
}
