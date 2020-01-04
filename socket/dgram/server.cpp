#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "socket_dgram.h"

int main(){

    // create the socket
    int sfd = socket(AF_UNIX,SOCK_DGRAM,0);
    if(sfd < 0)
        handle_error("SOCKET");
    
    //assign a unique name/address for your socket
    //bind creates the actual file that we unlink at cleaning up step
    sockaddr_un server_sockaddr, client_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr));
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path,SOCKET_SRVR_PTH_NAME,sizeof(server_sockaddr.sun_path));
    if (bind(sfd,(sockaddr *)&server_sockaddr,sizeof(sockaddr_un)) < 0 )
        handle_error("BIND");


    

    char buffer[MAX_BUF_SIZE];
    while(1){
        //read
        socklen_t client_sockaddr_len;
        ssize_t cnt = recvfrom(sfd,buffer,MAX_BUF_SIZE,0,(sockaddr *)&client_sockaddr,&client_sockaddr_len);
        if(cnt == 0) /*enter putton from stdin is translated to EOF, cnt is zero*/
            break;
        else if (cnt < 0)
            handle_error("RECVFROM");
        printf("[SERVER] message received:\n");
        write(STDOUT_FILENO,buffer,cnt);
        //write
        printf("[SERVER] enter a message to send to server through socket \n");
        cnt = read(STDIN_FILENO,buffer,MAX_BUF_SIZE);
        if(cnt == 1) /*enter putton from stdin is 1 */
            break;
        cnt = sendto(sfd,buffer,cnt,MSG_CONFIRM,(sockaddr *)&client_sockaddr,client_sockaddr_len);
        if (cnt < 0)
            handle_error("SEND_TO");
    }

    //clean up
    close(sfd);
    unlink(SOCKET_SRVR_PTH_NAME);
    
    return 0;
}