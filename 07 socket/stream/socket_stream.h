#define handle_error(msg)  \
            do { perror(msg); _exit(1); } while (0)

#define SOCKET_SRVR_PTH_NAME "/tmp/srvr_socket"
#define MAX_BUF_SIZE 1024
