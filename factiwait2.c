//
// Created by root on 5/6/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
/*
* These are all headers you need!
*/

//gcc -std=c99 -Wall -pedantic -o activate factiwait.c
//./activate ls -l -a . \; date
//./activate ls -l -a /tmp \; sleep 5

//pipeline <PROGRAM> [<PARAM>. . .] \| <PROGRAM> [<PARAM>. . .] \| <PROGRAM> [<PARAM>. . .]
//$pipeline ps -aux \| cat \| grep bash
//$pipeline ls -l /tmp/ \| cat \| wc -l -c -m

static int o = 0;
int findDelimiter(char *input[], int currentPosition, int limit){
    int output;
    for (int i = currentPosition; i < limit; i++) {
        if (strcmp(input[i], "|") == 0) { // in command line: \|
            output = i;
            o++;
            return output;//break;
        }
    }
    printf("delim went wrong; o: %d \n", o);
    exit(1);
}
void fillParameters(int firstSlot, int lastSlot, char **toBeFilled, char **provider ){
    int argumentIndex = 0;
    for (int i = firstSlot; i < lastSlot; i++) {
        toBeFilled[argumentIndex] = provider[i];
        argumentIndex++;
    }
    toBeFilled[argumentIndex] = NULL;
    //argumentIndex = 0;
}

int main(int argc, char *argv[]) {
    char **arguments1= NULL; //arguments1[]
    char **arguments2= NULL; //arguments2[]
    char **arguments3= NULL; //arguments3[]
    //int delimiter_index3 = -1;
    //int delimiter_index2 = -1;
    //int delimiter_index1 = -1;

    int border1 = 1;
    int border2 = 1;
    int size = -1;

    /* First PROGRAMM Preperation */
    border2 = findDelimiter(argv, border1, argc);
    size = border2;
    //arguments1 = malloc((border2) * sizeof(char *) ); //A spot for NULL should already be included here
    if ((arguments1 = malloc((size) * sizeof(char *) )) == NULL) {
        perror("malloc failed for arguments1");
        exit(1);
    }
    fillParameters(border1, border2, arguments1, argv);

    /* Second PROGRAMM Preperation */
    border1 = border2+1;
    border2 = findDelimiter(argv, border1, argc);
    size= (border2-border1+1);
    if ((arguments2 = malloc( (size)* sizeof(char *) )) == NULL) {
        perror("malloc failed for arguments2");
        exit(1);
    }
    fillParameters(border1, border2, arguments2, argv);

    /* Third PROGRAMM Preperation */
    border1 = border2+1;
    border2 = argc -1;
    size = (border2-border1+1);
    if ((arguments3 = malloc( (size)* sizeof(char *) )) == NULL) {
        perror("malloc failed for arguments3");
        exit(1);
    }
    fillParameters(border1, border2, arguments3, argv);

    /* PIDs */
    pid_t pid, wpid;
    int pfd[2]; //child 1 to child 2
    int pfd2[2]; //child 2 to child 3



    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    }
    if (pid == 0) { /* first child */
        execvp(arguments1[0], arguments1);
        perror("Command failed: ");
        exit(1);
    } else { /* parent */
        // does nothing here!
        //printf("Parent does nothing here\n");
    }

    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    }
    if (pid == 0) { /* second child */
        execvp(arguments2[0], arguments2);
        perror("Command failed: ");
        exit(1);
    }
    else { /* parent */
        int status;

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
