#include "linux_socket.h"

int PosixSocket::create_af_unix_socket( const char * file_path, int n )
{
    const int socket_id = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socket_id == -1) {
        perror("client socket");
        return -1;
    }

    /* Ensure portable by resetting all to zero */
    memset(&name, 0, sizeof(name));

    /* AF_UNIX */
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    return socket_id;
}

bool PosixSocket::connect( int socket_handle,
                           const struct socketaddr * name, size_t name_length )
{
    return ::connect(data_socket, &name, sizeof(name)) != -1;
}

bool PosixSocket::send( int socket_handle, const char * msg, size_t len )
{
    return ::send(data_socket, msg, len, MSG_DONTWAIT) != -1;
}

bool PosixSocket::recv( int socket_handle, char * buffer, size_t& len )
{
    return ::recv(socket_handle, buffer, len, MSG_DONTWAIT) != -1;
}

int PosixSocket::poll( int socket_handle )
{
    struct pollfd pfd;
    pfd.fd = socket_handle;
    pfd.events = POLLIN | POLLOUT;
    pfd.revents = 0;
    return ::poll(&pfd, 1, 0);
}

int PosixSocket::accept( int socket_handle,
                         const struct sockaddr *addr, size_t len)
{
    return ::accept(socket_handle, addr, len);
}

void PosixSocket::close(int socket_handle) {
    ::close(socket_handle);
}

int PosixSocket::bind( int socket_handle, const struct sockaddr *addr, size_t len)
{
    return ::bind(socket_handle, addr, len);
}

int PosixSocket::listen( int socket_handle, int connection_pool_size )
{
    return ::listen(socket_handle, connection_pool_size);
}