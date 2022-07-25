#ifndef GENERIC_IPC_HANDLER_WINDOWS_SOCKET_H
#define GENERIC_IPC_HANDLER_WINDOWS_SOCKET_H

class WindowsSocket : public SocketImplementation {
    static int initialize(){
        WSADATA wsaData;
        int err = WSAStartup(MakeWord(2,2), &wsaData);
        if(err != 0)
        {
            printf("WSAStartup Failed: %d", err);
            return -1;
        }

    }
    static int finalize(){
        WSACleanup();
    }

    static int create_af_unix_socket( const char * file_path, int n )
    {

    }

    static bool set_non_blocking( int socket_handle )
    {
        unsigned long enable_non_blocking = 1;
        return ioctlsocket(fd, FIONBIO, &enable_non_blocking ) == 0;
    }

};


#endif //GENERIC_IPC_HANDLER_WINDOWS_SOCKET_H
