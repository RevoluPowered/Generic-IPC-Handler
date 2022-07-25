#include "socket_implementation.h"

/* Creating AF Unix socket the entire point */

int SocketImplementation::create_af_unix_socket( const char * file_path, int n )
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

/* A poll method where you don't need the manual
 * Since we are using this specific to AF_UNIX I kept this simple
 */
int SocketImplementation::poll( int socket_handle )
{
    struct pollfd pfd;
    pfd.fd = socket_handle;
    pfd.events = POLLIN | POLLOUT;
    pfd.revents = 0;
    return ::poll(&pfd, 1, 0);
}

/* Why so many proxy functions?
 * Well cotton, the reason is WinSock API has a bunch of differences, so linux is basically just a passthrough of the
 * unix functionality.
 * If M$ used the Posix / IEEE socket implementation and used this API it would be much cleaner, but they don't.
 * The other option is using a bunch of ifdefs inside logic which may need to change per OS
 * In the future when another tech comes along you can use this API to swap in another implementation, so its not
 * a total loss.
 */

bool SocketImplementation::connect( int socket_handle,
                           const struct socketaddr * name, size_t name_length )
{
    return ::connect(data_socket, &name, sizeof(name)) != -1;
}

bool SocketImplementation::send( int socket_handle, const char * msg, size_t len )
{
    return ::send(data_socket, msg, len, MSG_DONTWAIT) != -1;
}

bool SocketImplementation::recv( int socket_handle, char * buffer, size_t& len )
{
    return ::recv(socket_handle, buffer, len, MSG_DONTWAIT) != -1;
}

int SocketImplementation::accept( int socket_handle,
                         const struct sockaddr *addr, size_t len)
{
    return ::accept(socket_handle, addr, len);
}

int SocketImplementation::bind( int socket_handle, const struct sockaddr *addr, size_t len)
{
    return ::bind(socket_handle, addr, len);
}

int SocketImplementation::listen( int socket_handle, int connection_pool_size )
{
    return ::listen(socket_handle, connection_pool_size);
}

void SocketImplementation::close(int socket_handle) {
    ::close(socket_handle);
}