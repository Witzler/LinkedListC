//
// Created by root on 5/6/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

void main(void){
    pid_t pid, wpid;
    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    } else if (pid == 0) { /* first child */
        char *arguments[]={"ls", "-l", "-a", NULL};
        printf("child: PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments[0], arguments);
        perror("Command failed: ");
        exit(1);
    } else { /* parent */
        // does nothing here!
    }
    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    } else if (pid == 0) { /* second child */
        char *arguments[]={"ls", "-l", "-a", "/foobar", NULL};
        printf("child: PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments[0], arguments);
        perror("Command failed: ");
        exit(1);
    } else { /* parent */
        int status;
        printf("parent: PID [%i] CPID [%i]\n", getpid(), pid);
        printf("Waiting for child .... \n");
        for (int i=0; i<2; i++) { // wait for 2 processes!
            wpid=waitpid(-1, &status, 0);
            if(WIFEXITED(status)){
                printf("[%i] child (PID: %i) terminated via exit(%d)\n", i, wpid, WEXITSTATUS(status));
            } else if(WIFSIGNALED(status)) {
                printf("[%i] child (PID: %i) terminated via signal [%d]\n", i, wpid, WTERMSIG(status));
            } else {
                printf("[%i] something different happened to the child!\n",i );
            }
        }
    }
}