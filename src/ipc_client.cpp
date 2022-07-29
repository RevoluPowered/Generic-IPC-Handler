#include <iostream>
#include "ipc.h"


void Receive( const char * string, int strlen )
{
    std::cout << "Got : " << string << "length: " << strlen << std::endl;
}

int main() {
    IPCClient client;
    client.add_receive_callback(&Receive);
    char hello[] = { "client_init\0"};
    if(client.setup_one_shot(hello, strlen(hello)))
    {
        client.poll_update();
    }
    return 0;
}
