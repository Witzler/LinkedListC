#define _GNU_SOURCE //pipe2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>   // für O_CLOEXEC



#define READ_END 0
#define WRITE_END 1
void main(void){
    pid_t pid;
    int pfd[2]; /* Pipe file descriptors */
    size_t num_read;
    char buf[1024]={0};
    if (pipe2(pfd,O_CLOEXEC) == -1) { /* Create the pipe */
        perror("Pipe failed: "); exit(1);
    }
    if ((pid = fork()) == -1) { /* fork error */
        perror("fork failed"); exit(1);
    } else if (pid == 0) { /* fork child */
        close(pfd[WRITE_END]);
        for (;;) {
            memset(buf, 0, sizeof(buf));
            if((num_read=read(pfd[READ_END], buf, 1024))==-1)
                exit(1);
            else if (num_read == 0)
                break;
            else
                printf(" child read: [%s]\n", buf);
        }
        exit(0);
    } else { /* fork parent */
        close(pfd[READ_END]);
        for (int i=0; i<50;i++){
            sprintf(buf, "%i ", i);
            printf("parent write: [%s]\n", buf);
            if (write(pfd[WRITE_END], buf, strlen(buf)) != strlen(buf))
                perror("parent write error");
        }
        if (close(pfd[WRITE_END]) == -1) /* Child will see EOF */
            perror("parent close pipe");
        wait(NULL);
    }
}