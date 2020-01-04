#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "shm.h"


#define OPEN_FLAGS     O_CREAT | O_RDWR
#define CREATE_PERMISSONS     S_IRUSR | S_IWUSR

void failed(const char * msg){
    write(STDOUT_FILENO,msg,strlen(msg));
    _exit (1);
}

void dump_file(){
    printf("[SERVER] content of the file is ..\n");
    int fd = open(FILE_PATH,O_RDONLY);
    char buffer[MAX_BUF_SIZE];
    read(fd,buffer,MAX_BUF_SIZE);
    write(STDOUT_FILENO,buffer,MAX_BUF_SIZE);
    close(fd);
}

int main(){

    printf("Warning: this example is not client/server ipc example, see notes\n");
    int fd = open(FILE_PATH,OPEN_FLAGS, CREATE_PERMISSONS);
    if(fd < 0)
        failed("[SERVER] couldn`t open file\n");

    void * shmp = mmap(NULL , sizeof(shm_buffer),PROT_READ | PROT_WRITE,MAP_SHARED ,fd, 0);

    if(shmp < 0)
        failed("[SERVER] couldn`t create shared mmap\n");
    

    // resizing the file, otherwise expect unknown behaviour
    ftruncate(fd,sizeof(shm_buffer));
    close(fd);
    shm_buffer * shm_ptr = reinterpret_cast<shm_buffer *>(shmp);


    // writing to the shared memory -- then syncing with the file
    printf("[SERVER] please enter message -- writing to shm:\n");
    read(STDIN_FILENO,shm_ptr->buf,MAX_BUF_SIZE);
    msync(shmp,sizeof(shm_buffer),MS_SYNC);

    dump_file();



    // writing to the file -- then dumping the shared memory
    printf("[SERVER] please enter message -- writing to file:\n");
    fd = open(FILE_PATH,O_WRONLY);
    char buffer[MAX_BUF_SIZE];
    read(STDIN_FILENO,buffer,MAX_BUF_SIZE);
    lseek(fd,0,0);
    write(fd,buffer,strlen(buffer));
    msync(shmp,sizeof(shm_buffer),MS_SYNC);
    close(fd);

    printf("[SERVER] content of the shared memory is .. %s.",shm_ptr->buf);

    dump_file();

    //clean up
    munmap(shmp,sizeof(shm_buffer));
    unlink(FILE_PATH);
    
 return 0;   
}