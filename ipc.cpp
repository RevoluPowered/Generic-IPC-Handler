#include "IPC.h"

IPCBase::IPCBase(){}
IPCBase::~IPCBase(){}
IPCBase::add_receive_callback( CallbackDefinition callback )
{
    // func pointer to func pointer.
    activeCallback = callback;
}

void IPCClient::setup()
{
    // Hello world MacOS return point. ;)
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1) {
        return;
    }

    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    int OK = connect(data_socket, (const struct socketaddr *) &addr, sizeof(addr));
    if(OK == -1)
    {
        return;
    }
}

void IPCClient::poll()
{
    // unlikely to be used as (right now) later if we need a backward pipe.
}

void IPCServer::setup()
{
    // Hello world MacOS return point. ;)
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1) {
        return;
    }

    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    int OK = bind(connection_socket, &name, sizeof(name));
    if (OK == -1) {
        return;
    }

    OK = listen(connection_socket, 32); // assume spamming of new connections
    if (OK == -1) {
        return;
    }
}

/* We process and read the buffer once per tick */
void IPCServer::poll()
{
    data_socket = accept(connection_socket, NULL, NULL);
    if( data_socket == -1 ){
        return; // do nothing.
    }
    
    int OK = read(data_socket, buffer, sizeof(buffer));
    if(OK == -1){
        return;
    }

    /* Buffer must be null terminated */
    buffer[sizeof(buffer) - 1] = 0;

    /* Pass data to the application hooked in */
    if(activeCallback)
    {
        activeCallback(buffer, sizeof(buffer));
    }

    /* Zero buffer */
    buffer = {0};

    /* Zero buffer reply means we got the message */
    OK = write(data_socket, buffer, sizeof(buffer);

    close(connection_socket);
    unlink(SOCKET_NAME);
}
