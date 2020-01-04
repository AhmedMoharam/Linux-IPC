#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "fifo-common.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main(){
    printf("[CLIENT] waiting for the server ...\n");
    int fifod;
    
    //if fifo is not yet created, loop infinitly
    //if fifo exists open is blocking untill server opens for read
    while ((fifod = open(FIFO_PATH,O_WRONLY)) < 0 ){
        int errsv = errno;
        if (errsv != ENOENT){
            return 1;
        }
    }
    sleep(1);
    printf("[CLIENT] please enter the message, to send:\n");
    
    //get message from stdin
    std::string message;
    getline(std::cin,message);
    
    //writing message to the fifo
    write(fifod,message.c_str(), message.length());
    close(fifod);
 return 0;   
}