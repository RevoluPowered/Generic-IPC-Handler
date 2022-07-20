#include "ipc.h"
#include <iostream>
IPCBase::IPCBase(){}
IPCBase::~IPCBase(){}

IPCClient::IPCClient(){}
IPCClient::~IPCClient(){
    close(data_socket);
}

IPCServer::IPCServer(){}
IPCServer::~IPCServer(){
    close(connection_socket);
    unlink(SOCKET_NAME);
}


// called to register the only callback for when data arrives
void IPCBase::add_receive_callback( CallbackDefinition callback )
{
    // func pointer to func pointer.
    activeCallback = callback;
}

bool IPCClient::setup()
{
    printf("Starting socket\n");
    // Hello world MacOS return point. ;)
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(data_socket == -1) {
        perror("client socket");
        return false;
    }

    int flags = fcntl(data_socket, F_GETFD );
    int ret = fcntl(data_socket, F_SETFD, flags | O_NONBLOCK);
    printf("state %d\n", ret);

    printf("configuring socket type and path\n");
    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
    printf("waiting for connection\n");
    int OK = connect(data_socket, (const struct sockaddr *) &name, sizeof(name));
    if (OK == -1) {
        perror("client connect");
        return false;
    }
    else
    {
        std::cout << "Connected to server" << std::endl;
    }

    printf("waiting for write of client_init [%d] %s \n", __LINE__, __FILE__);

    char hello[] = "client_init\0";
    OK = write(data_socket, hello, sizeof(hello));
    if(OK == -1)
    {
        perror("cant send message");
        return false;
    }

	printf("Waiting for read of client_init [%d] %s\n", __LINE__, __FILE__ );

	OK = read(data_socket, buffer, BufferSize);
	if(OK == -1)
	{
		perror("read client socket");
		close(data_socket);
		return false;
	}

	if(strncmp(hello, buffer, BufferSize) != 0)
	{
        printf("[%d] %s client buffer:\n %s\n %s\n",
               __LINE__,
               __func__,
               hello,
               buffer);
		perror("comparison buffer result wrong client\n");
		close(data_socket);
		return false;
	}

    return true;
}

bool IPCClient::setup_one_shot( const char *str, int n ) {
	printf("Starting socket\n");
	// Hello world MacOS return point. ;)
	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(data_socket == -1) {
		perror("client socket");
		return false;
	}

    int flags = fcntl(data_socket, F_GETFD );
    int ret = fcntl(data_socket, F_SETFD, flags | O_NONBLOCK);
    printf("state %d\n", ret);

	printf("configuring socket type and path\n");
	/* Ensure portable by resetting all to zero */
	memset(&name, 0, sizeof(name));

	/* AF_UNIX */
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
	printf("waiting for connection\n");
	int OK = connect(data_socket, (const struct sockaddr *) &name, sizeof(name));
	if (OK == -1) {
		perror("client connect");
		return false;
	}
	else
	{
		printf("Connected to server");
	}

	printf("waiting for write of client_init [%d] %s \n", __LINE__, __FILE__);

	OK = write(data_socket, str, n);
	if(OK == -1)
	{
		perror("cant send message");
		close(data_socket);
		return false;
	}

	printf("Waiting for read of client_init [%d] %s\n", __LINE__, __FILE__ );

	/* Non blocking */
	OK = read(data_socket, buffer, BufferSize);
	if(OK == -1 || OK == EWOULDBLOCK || OK == O_NONBLOCK)
	{
		perror("read client socket");
		close(data_socket);
		return false;
	}

    buffer[BufferSize - 1] = 0;
	if(strncmp(str, buffer, BufferSize) != 0)
	{
		perror("comparison buffer result wrong client");
		close(data_socket);
		return false;
	}

	close(data_socket);

	return true;
}

void IPCClient::send_message( const char * str, int n )
{
	char hello[] = "client_some_message\0";
	int OK = write(data_socket, hello, sizeof(hello) );
	if(OK == -1)
	{
		perror("write");
	}
}

bool IPCClient::poll()
{
//    // unlikely to be used as (right now) later if we need a backward pipe.
//    const char str[] = "Hello World from the Client!\0";
//
//    int OK = write(data_socket, str, sizeof(str));
//    if(OK == -1)
//    {
//       perror("write");
//       return false;
//    }

    return true;
}

bool IPCServer::setup()
{
    unlink(SOCKET_NAME);
    printf("Setting up server connection socket\n");
    // Hello world MacOS return point. ;)
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connection_socket == -1) {
        perror("socket");
        return false;
    }


//    int flags = 1;
//    ioctl(connection_socket, FIOBIO, &flags);
    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;

    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    printf("trying to bind connection\n");
    int OK = bind(connection_socket, (const struct sockaddr *) &name,sizeof(name));
    if (OK == -1) {
        perror("bind");
        return false;
    }

    int flags = fcntl(connection_socket, F_GETFD );
    int ret = fcntl(connection_socket, F_SETFD, flags | O_NONBLOCK);
    printf("state %d\n", ret);

    printf("Starting listen logic\n");
    OK = listen(connection_socket, 8); // assume spamming of new connections
    if (OK == -1) {
        perror("listen");
        return false;
    }

    printf("started listening for new connections\n");
    return true;
}

/* We process and read the buffer once per tick */
bool IPCServer::poll()
{
    // server only
    data_socket = accept(connection_socket, NULL, NULL);

    // both are checked for portability to all OS's.
    if(data_socket == EWOULDBLOCK || data_socket == EAGAIN) {
        return true; // not an error
    } else if (data_socket == -1) {
        perror("socket open failed");
        return false;
    } else {
        printf("Server accepted connection\n");
    }

    /* end server only. */
    int OK = read(data_socket, buffer, BufferSize);
    if (OK == -1) {
        perror("server read");
        return false;
    }
    else
    {
        printf("socket read success\n");
    }

    /* Buffer must be null terminated */
    buffer[BufferSize - 1] = 0;

    OK = write(data_socket, buffer, BufferSize);
    if(OK == -1)
    {
        perror("cant send message");
        close(data_socket);
        return false;
    }

    /* Pass data to the application hooked in */
    printf("Received message from client: %s\n", buffer);
    if (activeCallback) {
        activeCallback(buffer, strlen(buffer));
    }

    close(data_socket);

    return true;
}
