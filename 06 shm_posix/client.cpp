
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include "shm.h"

void failed(const char * msg){
    printf("%s",msg);
    _exit(1);
}

int main(){
    int fd;
    while ((fd = shm_open(SHM_POSIX_NAME,O_RDWR,0)) < 0) {
        int errsv = errno;
        if(errsv != ENOENT )
            failed("[CLIENT] couldn`t open shared memory object\n");
    }
    
    struct stat shm_stat;
    fstat(fd,&shm_stat);
    
    void * shmp = mmap(NULL,shm_stat.st_size,PROT_READ | PROT_WRITE ,MAP_SHARED,fd,0);
    printf("shared memory ptr: %p\n", shmp);
    shmbuf * shm_ptr = reinterpret_cast<shmbuf *>(shmp);

    while(1){
        sem_wait(&shm_ptr->rsem);
        if(shm_ptr->cnt == 1)
            break;
        printf("[CLIENT] reading from shared memory ...\n");
        write(STDOUT_FILENO,shm_ptr->buf,shm_ptr->cnt);
        sem_post(&shm_ptr->wsem);
    }
    close(fd);
    sem_post(&shm_ptr->wsem);

    return 0;
}