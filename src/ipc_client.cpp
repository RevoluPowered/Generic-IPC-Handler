#include <iostream>
#include "ipc.h"


void Receive( const char * string, int strlen )
{
    std::cout << "Got : " << string << "length: " << strlen << std::endl;
}

int main() {
    IPCClient client;
    client.add_receive_callback(&Receive);
    if(client.setup())
    {
        client.poll_update();
    }
    return 0;
}
