#ifndef GENERIC_IPC_HANDLER_SOCKET_IMPLEMENTATION_H
#define GENERIC_IPC_HANDLER_SOCKET_IMPLEMENTATION_H

struct SocketImplementation
{
    static void initialize() = 0;
    static void finalize() = 0;
    static bool create_af_unix_socket(const char * file_path, int n) = 0;
    /*
     * Sets the socket into non-blocking mode for read/write operations
     * listen() is non-blocking on Linux but not on Mac, on Mac you must poll() first.
     * Windows has a different handler for setting the socket to non-blocking
     * Unix, Linux and Mac uses fcntl.
    */
    static int set_non_blocking( int socket_handle ) = 0;
    static int connect(int socket_handle, const struct socketaddr * name, size_t name_length ) = 0;
    static int send(int socket_handle, const char * msg, size_t len) = 0;
    static int recv(int socket_handle, char * buffer, size_t &len ) = 0;
    static int poll( int socket_handle ) = 0;
    static int accept( int socket_handle, const struct sockaddr *addr, size_t len) = 0;
    static void close( int socket_handle ) = 0;
    static int bind( int socket_handle, const struct sockaddr *addr, size_t len) = 0;
    static int listen( int socket_handle, int connection_pool_size) = 0;
};

#endif //GENERIC_IPC_HANDLER_SOCKET_IMPLEMENTATION_H
