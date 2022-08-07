#include "ipc.h"

#ifdef _WIN32
#define SOCKET_NAME "c:/som74yhe.socket"
#else
#define SOCKET_NAME "/tmp/som74yhe.socket"
#endif


void Receive( const char * string, int strlen )
{
    printf("server got message: %s\n", string);
}

int main() {
    IPCServer server;
    printf("Starting server\n");
    server.add_receive_callback(&Receive);
    if( server.setup(SOCKET_NAME) )
    {
        printf("Server setup completed, starting polling\n");
        while( server.poll_update() ){
            // do nothing
        };
    }
    return 0;
}
