#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include "socket_dgram.h"

int main(){
    //create socket of the same type as the server
    int fd = socket(AF_UNIX,SOCK_DGRAM,0);
    if(fd < 0)
        handle_error("CLIENT SOCKET");

    //connect your created socket fd to the passive server by knowing its address structure
    sockaddr_un server_sockaddr;
    memset(&server_sockaddr,0,sizeof(server_sockaddr));
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path,SOCKET_SRVR_PTH_NAME,sizeof(server_sockaddr.sun_path));

    while(1) {
        //write
        printf("[CLIENT] enter a message to send to server through socket \n");
        char buffer[MAX_BUF_SIZE];
        ssize_t cnt = read(STDIN_FILENO,buffer,MAX_BUF_SIZE);
        if(cnt == 1) /*enter putton from stdin is 1 */
            break;
        cnt = sendto(fd,buffer,cnt,0,(sockaddr *)&server_sockaddr,sizeof(server_sockaddr));
        if(cnt < 0)
            handle_error("SEND_TO");

        //read
        socklen_t server_len;
        cnt = recvfrom(fd,buffer,MAX_BUF_SIZE,MSG_WAITALL,(sockaddr *)&server_sockaddr,&server_len); /*no longer need to refill server address structure and length*/
        if(cnt == 0) /*enter putton from stdin is translated to EOF, cnt is zero*/
            break;
        else if (cnt < 0)
            handle_error("RECVFROM");
        printf("[CLIENT] message received:\n");
        write(STDOUT_FILENO,buffer,cnt);
    }

    close(fd);

    return 0;
}