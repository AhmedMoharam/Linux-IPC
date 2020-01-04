#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "socket_stream.h"

int main(){

    // create the socket
    int sfd = socket(AF_UNIX,SOCK_STREAM,0);
    if(sfd < 0)
        handle_error("SOCKET");
    
    //assign a unique name for your socket
    //bind creates the actual file that we unlink at cleaning up step
    sockaddr_un server_sockaddr, client_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr));
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path,SOCKET_SRVR_PTH_NAME,sizeof(server_sockaddr.sun_path));
    if (bind(sfd,(sockaddr *)&server_sockaddr,sizeof(sockaddr_un)) < 0 )
        handle_error("BIND");

    //mark your socket as a passive socket, a socket waiting for connection, set #ofclients
    // add your socket to the pool of pasive sockets
    if(listen(sfd,1 /*one client*/) < 0 )
        handle_error("LISTEN");

    //accept is blocking , untill connections arrives, you can loop to accpet connections one by one
    // upto number of clients stated at listen
    // accept returns the file descriptor of the client requesting connction and its address struct is 
    // filled alongside with its length
    // must fill client socket addr size with address size before sending
    socklen_t client_addr_size = sizeof(sockaddr_un);
    int cfd = accept(sfd,(sockaddr *)&client_sockaddr,&client_addr_size);
    if(cfd < 0 )
        handle_error("ACCEPT");
    

    char buf[MAX_BUF_SIZE];
    while(1){
        //read
        int i=0;
        write(cfd,&i,4);
        ssize_t cnt = read(cfd,buf,MAX_BUF_SIZE);
        if(cnt == 0) /*enter putton from stdin is translated to EOF, cnt is zero*/
            break;
        printf("[SERVER] message received:\n");
        write(STDOUT_FILENO,buf,cnt);
        //write
        printf("[SERVER] enter a message to send to server through socket \n");
        cnt = read(STDIN_FILENO,buf,MAX_BUF_SIZE);
        if(cnt == 1) /*enter putton from stdin is 1 */
            break;
        write(cfd,buf,cnt);
    }

    //clean up
    close(sfd);
    unlink(SOCKET_SRVR_PTH_NAME);
    
    return 0;
}