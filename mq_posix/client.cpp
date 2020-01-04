#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include "mq-common.h"
#include <iostream>


int main(){
    printf("[CLIENT] waiting for the server ...\n");
    //open mq for write
    mqd_t mq_handle;
    mq_handle = mq_open (MQUEUE_PATH, O_WRONLY);

    printf("[CLIENT] please enter the message to send:\n");
    //get message from stdin to send to server
    std::string message;
    getline(std::cin, message);


    //send the message to queue
    //message must be not larger than MAX_MSG_SIZE or we get error EMSGSIZE
    if(message.length() > MAX_MSG_SIZE)
        message = message.substr(0,MAX_MSG_SIZE -1);

    printf("[CLIENT] sending:%s\n", message.c_str());

    if (mq_send(mq_handle,message.c_str(), message.length(),0) < 0){
        printf("[CLIENT] error sending the message.\n");
        mq_close(mq_handle);
        return 1;
    }

    //close the queue
    mq_close(mq_handle);

 return 0;   
}