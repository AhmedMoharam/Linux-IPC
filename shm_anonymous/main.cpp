#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "shm.h"
#include <semaphore.h>


int main(){

    void * shmp= mmap( NULL ,sizeof(shmbuf), PROT_WRITE | PROT_READ  ,MAP_ANONYMOUS  | MAP_SHARED , -1,0 );
    
    pid_t pid;
    shmbuf * shm_ptr = reinterpret_cast<shmbuf *>(shmp);
    sem_init(&shm_ptr->wsem,1,1); // writer starts first (sem_wait blocks if sem is 0 and unblocks once it post)
    sem_init(&shm_ptr->rsem,1,0); // reader cannot start untill writer finishes

    if ((pid = fork()) < 0){
        printf("ERROR: could not fork\n");
       return 1;
    } else if (pid == 0){ //child //writer
        while(1){
            sem_wait(&shm_ptr->wsem);
            printf("please enter your message:\n");
            shm_ptr->cnt = read(STDIN_FILENO,&shm_ptr->buf,BUF_SIZE);
            sem_post(&shm_ptr->rsem);
            if(shm_ptr->cnt == 1)
                break;
        }
        sem_wait(&shm_ptr->wsem); // wait for reader to finish reading and post writer

    } else { // parent //reader
        while(1){
            sem_wait(&shm_ptr->rsem);
            // do some reading from memory  
            if(shm_ptr->cnt == 1)
                break;
            printf("message read:\n");
            write(STDOUT_FILENO,&shm_ptr->buf,shm_ptr->cnt);
            sem_post(&shm_ptr->wsem);
        }
        sem_post(&shm_ptr->wsem); // posting writer to notify reader finish
        //clean up
        sem_destroy(&shm_ptr->wsem);
        sem_destroy(&shm_ptr->rsem);
        munmap(shmp,sizeof(shmbuf));
        wait(NULL); //wait for child
    }


    return 0;
}