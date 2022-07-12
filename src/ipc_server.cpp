#include <iostream>
#include "ipc.h"


void Receive( const char * string, int strlen )
{
    std::cout << "Got : " << string << "length: " << strlen << std::endl;
}

int main() {
    IPCServer server;
    printf("Starting server\n");
    server.add_receive_callback(&Receive);
    if( server.setup() )
    {
        printf("Server setup completed, starting polling\n");
        while( server.poll() ){
            // do nothing
        };
    }
    return 0;
}
