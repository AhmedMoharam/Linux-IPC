#include <semaphore.h>

#define BUF_SIZE 1024
#define SHM_POSIX_NAME "/shm_posix"

struct shmbuf { //Buffer in shared memory
    sem_t wsem;         //writer semaphore (unnamed)
    sem_t rsem;         //reader semaphore (unnamed
    int cnt;            //number of bytes used in buf
    char buf[BUF_SIZE]; //data being transfered
};