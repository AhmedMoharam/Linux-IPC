#include <semaphore.h>

#define FILE_PATH  "/tmp/shm_file" 
#define MAX_BUF_SIZE    1024

struct shm_buffer {
    sem_t wsem;
    sem_t rsem;
    int cnt;
    char buf[MAX_BUF_SIZE];
};