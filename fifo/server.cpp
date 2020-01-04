#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "fifo-common.h"


int main(){
    // creating FIFO
    if(mkfifo(FIFO_PATH,S_IRUSR | S_IWUSR ) < 0){
        int errsv = errno;
        if(errsv == EEXIST ){
            printf("[SERVER] FIFO exists\n");
        } else {
            printf("[SERVER] couldn`t create fifo\n");
            return 1;
        }

    } 
    //opening FIFO for read
    int fifod = open(FIFO_PATH, O_RDONLY);
    printf("[SERVER] ready to read and serve ...\n");
    char c;
    std::string message;
    
    //reading untill EOF is recieved
    while(read(fifod,&c,1) > 0){
        message += c;
    }

    //printing message received
    printf("[SERVER] message received:%s\n", message.c_str());

    // clean up (deleting) FIFO
    unlink(FIFO_PATH);
 return 0;   
}