#define handle_error(msg)  \
            do { perror(msg); _exit(1); } while (0)

#define MAX_BUF_SIZE 1024
#define PORT     8080 
