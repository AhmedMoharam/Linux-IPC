#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include "socket_stream.h"

int main(){
    //create socket of the same type as the server
    int fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(fd < 0)
        handle_error("CLIENT SOCKET");

    //connect your created socket fd to the passive server by knowing its address structure
    sockaddr_un server_sockaddr;
    memset(&server_sockaddr,0,sizeof(server_sockaddr));
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path,SOCKET_SRVR_PTH_NAME,sizeof(server_sockaddr.sun_path));
    while(connect(fd,(sockaddr *)&server_sockaddr,sizeof(server_sockaddr)) < 0){
        int errsv = errno;
        if(errsv != ENOENT && errsv != ECONNREFUSED) /*wait for the server to bind and listen*/
            handle_error("CONNECT");
    }

    while(1) {
        //write
        printf("[CLIENT] enter a message to send to server through socket \n");
        char message[MAX_BUF_SIZE];
        ssize_t cnt = read(STDIN_FILENO,message,MAX_BUF_SIZE);
        if(cnt == 1) /*enter putton from stdin is 1 */
            break;
        write(fd,message,cnt);

        //read
        cnt = read(fd,message,MAX_BUF_SIZE); 
        if(cnt == 0) /*enter putton from stdin is translated to EOF, cnt is zero*/
            break;
        printf("[CLIENT] message received:\n");
        write(STDOUT_FILENO,message,cnt);
    }

    close(fd);

    return 0;
}