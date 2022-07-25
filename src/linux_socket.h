#ifndef GENERIC_IPC_HANDLER_LINUX_SOCKET_H
#define GENERIC_IPC_HANDLER_LINUX_SOCKET_H

#include "socket_implementation.h

// Used to wrap sockets slightly cleaner to the API
class PosixSocket : public SocketImplementation
{
    static void initialize() {};
    static void finalize() {};
    static int create_af_unix_socket( const char * file_path, int n );

    static bool set_non_blocking( int socket_handle )
    {
        return fcntl(fd, F_SETFD, fcntl(data_socket, F_GETFD ) | O_NONBLOCK);
        if(ret == -1) {
            perror("fnctl error");
            return false;.
        }
        return true;
    }
};

#endif //GENERIC_IPC_HANDLER_LINUX_SOCKET_H
