// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "socket_dgram.h"

  
  
int main() { 
    int sockfd; 
    char buffer[MAX_BUF_SIZE]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        handle_error("SOCKET");
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
        handle_error("BIND");
    
      
    socklen_t cliaddr_len;
    ssize_t cnt; 

    while (1){
        //receive
        cnt = recvfrom(sockfd, buffer, MAX_BUF_SIZE, MSG_WAITALL, (struct  sockaddr *) &cliaddr,&cliaddr_len); 
        if(cnt == 1)
            break;
        else if (cnt < 0)
            handle_error("RECV_FROM");
        printf("[SERVER] message received:\n");
        write(STDOUT_FILENO,buffer,cnt);

        //send
        printf("[SERVER] please enter your message to send\n");
        cnt = read(STDIN_FILENO,buffer,MAX_BUF_SIZE);
        const void * data = buffer; //had to create it or send to fails (normal casting/reinterpret_casting fails)
        cnt = sendto(sockfd,data,cnt, MSG_CONFIRM, (const struct sockaddr *) &cliaddr,cliaddr_len); 
        if(cnt == 1)
            break;
        else if (cnt < 0)
            handle_error("SEND_TO");
    }

    close(sockfd);
    return 0; 
} 