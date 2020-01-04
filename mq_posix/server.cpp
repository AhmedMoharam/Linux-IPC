#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

#include <mqueue.h>
#include "mq-common.h"
#include <stdio.h>
#include <errno.h>


#define OPEN_FLAGS     O_CREAT | O_EXCL | O_RDONLY
#define CREATE_PERMISSONS     S_IRUSR | S_IWUSR

int main(){
    mqd_t mq_handle;
    struct mq_attr attr;
    int errsv;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;


    //creating posix message queue
    while( (mq_handle = mq_open(MQUEUE_PATH, OPEN_FLAGS , CREATE_PERMISSONS , &attr)) < 0 ){
        errsv = errno;
        if(errsv == EEXIST){
            mq_unlink(MQUEUE_PATH);
            continue;
        }
        printf("[SERVER] could not create a message queue\n");
        return 1;
    }
    printf("[SERVER] ready to read and serve ...\n");

    //read message till we get EOF by closing write discreptor
    char message[MAX_MSG_SIZE] = {};
    size_t size = mq_receive(mq_handle,message,MAX_MSG_SIZE,nullptr);
    if(size == -1){
        printf("[SERVER] error receiving data\n");
        mq_close(mq_handle);
        mq_unlink(MQUEUE_PATH);
        return 1;
    } else if (size < MAX_MSG_SIZE) {
        message[size] = 0;
    }
    
    //printing read message
    printf("[SERVER] received:%s\n",message);

    //cleaning message queue
    mq_close(mq_handle);
    mq_unlink(MQUEUE_PATH);
    
 return 0;   
}