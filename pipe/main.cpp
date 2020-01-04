#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>

/*
    stdin   0
    stdout  1
    stderr  2
    parent           child 
    fd[1]-->[PIPE]-->fd[0]
    processIn from pipe out fd[0] 
    processOut to pipe in fd[1] 
*/

int main(int argc, char * argv[]){
    int pipefd[2];
    pid_t  pid;
    std::string message;

    if(pipe(pipefd) != 0 ){
        printf("ERROR: could not create pipe\n");
       return 1;
    }

    if(( pid = fork()) < 0){ // fork failed
        printf("ERROR: could not fork\n");
       return 1;

    } else if ( pid == 0) { //On Child
        close(pipefd[1]); //closing the write (in) discriptor of the pipe on child
        printf("[Child] process ID: %d\n",getpid());
        printf("[Child] reads from the pipe\n");
        char c = '0';
        while(read(pipefd[0],&c, 1 ) > 0 ){ /* <0 at errors, == 0 at EOF */
           // write(STDOUT_FILENO, &c, 1);
            message +=c;
        }
        printf("[Child] message received is:%s\n", message.c_str());
        close(pipefd[0]);
    } else { // On parent
        close(pipefd[0]); //closing the read (out) discriptor of the pipe on parent
        printf("[Parent] process ID: %d\n",getpid());
        printf("[Parent] writes to the pipe\n");
        sleep(1);
        printf("[Parent] please enter the message, to send to the child:\n");
        getline(std::cin,message);
        printf("[Parent] sending message:%s ...\n", message.c_str());
        write(pipefd[1],message.c_str(),message.length());
        close(pipefd[1]); /* Reader will see EOF */
        wait(NULL); /* Wait for child */
    }
    return 0;
}