#include <iostream>
#include "ipc.h"


void Receive( const char * string, int strlen )
{
    std::cout << "Got : " << string << "length: " << strlen << std::endl;
}

int main() {
    IPCServer server;
    server.add_receive_callback(&Receive);
    IPCClient client;
    client.add_receive_callback(&Receive);
    if( server.setup() && client.setup() )
    {
        while(true)
        {
            server.poll();
            client.poll();
        }
    }
    return 0;
}
