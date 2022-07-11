#include <iostream>
#include "ipc.h"


void Receive( const char * string, int strlen )
{
    std::cout << "Got : " << string << "length: " << strlen << std::endl;
}

int main() {
    IPCServer server;
    server.add_receive_callback(&Receive);
    if( server.setup() )
    {
        server.poll();
    }
    return 0;
}
