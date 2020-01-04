// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "socket_dgram.h"
  
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAX_BUF_SIZE]; 
    struct sockaddr_in  servaddr; 
  
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        handle_error("SOCKET");
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    ssize_t cnt; 
    while (1){
        //send 
        printf("[CLIENT] please enter your message to send\n");
        cnt = read(STDIN_FILENO,buffer,MAX_BUF_SIZE);
        cnt = sendto(sockfd, buffer, cnt, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        if(cnt == 1)
            break;
        else if (cnt < 0)
            handle_error("SEND_TO");
        
        //receive
        socklen_t servaddr_len;
        cnt = recvfrom(sockfd, (char *)buffer, MAX_BUF_SIZE, MSG_WAITALL, (struct sockaddr *) &servaddr,&servaddr_len); 
        if(cnt == 1)
            break;
        else if (cnt < 0)
            handle_error("RECV_FROM");
        printf("[CLIENT] message received:\n");
        write(STDOUT_FILENO,buffer,cnt);
    }
  
    close(sockfd); 
    return 0; 
} 