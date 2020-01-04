#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "shm.h"


void failed(const char * msg){
    printf("%s",msg);
    _exit(1);
}
int main(){
    int fd = shm_open(SHM_POSIX_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRWXU );
    if (fd < 0)
        failed("[SERVER] couldn`t create shared memory object\n");
    ftruncate(fd,sizeof(shmbuf));

    void * shmp = mmap(NULL,sizeof(shmbuf),PROT_READ  | PROT_WRITE ,MAP_SHARED,fd,0);

    printf("shared memory ptr: %p\n", shmp);
    if(shmp < 0)
        failed("[SERVER] couldn`t allocate shared memory using mmap\n");
    
    shmbuf * shm_ptr = reinterpret_cast<shmbuf *> (shmp);

    sem_init(&shm_ptr->wsem,1,1);
    sem_init(&shm_ptr->rsem,1,0);

    while(1){
        sem_wait(&shm_ptr->wsem);
        printf("enter your message to write to shm:\n");
        shm_ptr->cnt = read(STDIN_FILENO,shm_ptr->buf,BUF_SIZE);
        sem_post(&shm_ptr->rsem);
        if(shm_ptr->cnt == 1)
            break;
    }
    
    sem_wait(&shm_ptr->wsem);
    //clean up
    sem_destroy(&shm_ptr->wsem);
    sem_destroy(&shm_ptr->rsem);
    munmap(shmp,sizeof(shmbuf));
    close(fd);
    shm_unlink(SHM_POSIX_NAME);
    
    return 0;
}