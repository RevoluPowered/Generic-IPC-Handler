#include "ipc.h"

IPCBase::IPCBase(){}
IPCBase::~IPCBase(){}

IPCClient::IPCClient(){}
IPCClient::~IPCClient(){}

IPCServer::IPCServer(){}
IPCServer::~IPCServer(){}


// called to register the only callback for when data arrives
void IPCBase::add_receive_callback( CallbackDefinition callback )
{
    // func pointer to func pointer.
    activeCallback = callback;
}

bool IPCClient::setup()
{
    // Hello world MacOS return point. ;)
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1) {
        perror("client socket");
        return true;
    }

    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    int OK = connect(data_socket, (const struct sockaddr *) &name, sizeof(name));
    if(OK == -1)
    {
        perror("client connect");
        return false;
    }

    return true;
}

bool IPCClient::poll()
{
    // unlikely to be used as (right now) later if we need a backward pipe.
    const char str[] = "Hello World from the Client!\0";
    
    int OK = write(data_socket, str, sizeof(str));
    if(OK == -1)
    {
       perror("write");
       return false; 
    }

    return true;
}

bool IPCServer::setup()
{
    unlink(SOCKET_NAME);
    // Hello world MacOS return point. ;)
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1) {
        return false;
    }

    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    int OK = bind(connection_socket, (const struct sockaddr *) &name,sizeof(name));
    if (OK == -1) {
        perror("bind");
        return false;
    }

    OK = listen(connection_socket, 32); // assume spamming of new connections
    if (OK == -1) {
        perror("listen");
        return false;
    }
    return true;
}

/* We process and read the buffer once per tick */
bool IPCServer::poll()
{
    // server only
    data_socket = accept(connection_socket, NULL, NULL);
    if( data_socket == -1 ){
        perror("socket accept");
        return false; // do nothing.
    }
    // end server only.
    
    int OK = read(data_socket, buffer, sizeof(buffer));
    if(OK == -1){
        perror("server read");
        return false;
    }

    /* Buffer must be null terminated */
    buffer[sizeof(buffer) - 1] = 0;

    /* Pass data to the application hooked in */
    if(activeCallback)
    {
        activeCallback(buffer, sizeof(buffer));
    }

    /* buffer reply means we got the message */
    OK = write(data_socket, buffer, sizeof(buffer));

    buffer[sizeof(buffer) -1] = 0;

    close(connection_socket);
    unlink(SOCKET_NAME);
    return true;
}
