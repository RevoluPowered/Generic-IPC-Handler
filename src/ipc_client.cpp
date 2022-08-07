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
    IPCClient client;
    client.add_receive_callback(&Receive);
    char hello[] = { "Hello World!\0"};
    if(client.setup_one_shot(SOCKET_NAME, hello, strlen(hello)))
    {
        client.poll_update();
    }
    return 0;
}
