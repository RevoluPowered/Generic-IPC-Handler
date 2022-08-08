# Generic-IPC-Handler
Trialing AF_UNIX sockets  (IPC) 

Initially this was started when I was trialing AF_UNIX sockets, but turned into a cross platform tool for passing data from one app to another.

it works on Mac/Windows and Linux, see src/tests/ for some example code.

# API notes
```cpp
IPCClient.setup_one_shot() // closes immediately
IPCClient.setup() // stays open
```

# To send data use 
```cpp
SocketImplementation::send / recv
```

Buffer is limited to 256 this will be removed later.


# Example
```cpp
#include "ipc.h"

#define SOCKET_NAME "ipc_test.socket"

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
```

