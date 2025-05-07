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


int main(int argc, char *argv[]) {
    /*
    * Do as you were told.
    * Split argv[1..n] by using a <DELIMITER> into:
    * <PROGRAM 1> [<PARAMETER>...]
    * <PROGRAM 2> [<PARAMETER>...]
    */
    char **arguments1= NULL; //arguments1[]
    int delimiter_index1 = -1;
    char **arguments2= NULL; //arguments2[]
    int delimiter_index2 = -1;

    /* First PROGRAMM Preperation */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ";") == 0) { // in command line: \;
            delimiter_index1 = i;
            break;
        }
    }
    arguments1 = malloc((delimiter_index1) * sizeof(char *) ); //A spot for NULL shall be included here
    if (arguments1 == NULL) {
        perror("malloc failed for arguments1");
        exit(1);
    }
    int argumentIndex = 0;
    for (int i = 1; i < delimiter_index1; i++) {
        arguments1[argumentIndex] = argv[i];
        argumentIndex++;
    }
    arguments1[delimiter_index1 -1] = NULL;

    /* Second PROGRAMM Preperation */
    delimiter_index2 = delimiter_index1+1;
    arguments2 = malloc((argc - delimiter_index2+1) * sizeof(char *));
    if (arguments2 == NULL) {
        perror("malloc failed for arguments2");
        exit(1);
    }
    argumentIndex = 0;
    for (int i = delimiter_index2; i < argc; i++) {
        arguments2[argumentIndex] = argv[i];
        argumentIndex++;
    }
    arguments2[delimiter_index2] = NULL;

    /*printf("=== arguments1 ===\n");
    for (int i = 0; arguments1[i] != NULL; i++) {
        printf("arguments1[%d] = \"%s\"\n", i, arguments1[i]);
    }
    int count = 0;
    while (arguments1[count] != NULL) {
        count++;
    }
    printf("site1: %d\n",count);
    printf("=== arguments2 ===\n");
    for (int i = 0; arguments2[i] != NULL; i++) {
        printf("arguments2[%d] = \"%s\"\n", i, arguments2[i]);
    }
    count = 0;
    while (arguments2[count] != NULL) {
        count++;
    }
    printf("site2: %d\n",count); */

    /* PIDs */
    pid_t pid, wpid;

    /*
    * create first child (fork()) and execute
    * <PROGRAM 1> [<PARAMETER>...] with exec*()
    */
    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    }
    if (pid == 0) { /* first child */
        //char *arguments[]={"ls", "-l", "-a", NULL};
        printf("child1: PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments1[0], arguments1);
        perror("Command failed: ");
        exit(1);
    } else { /* parent */
        // does nothing here!
        printf("Parent does nothing here\n");
    }


    /*
    * create second child (fork()) and execute
    * <PROGRAM 1> [<PARAMETER>...] with exec*()
    */
    if ((pid = fork()) == -1) { /* error */
        perror("Fork failed: ");
        exit(1);
    }
    if (pid == 0) { /* second child */
        //char *arguments[]={"ls", "-l", "-a", "/foobar", NULL};
        printf("child2: PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments2[0], arguments2);
        perror("Command failed: ");
        exit(1);
    }
    /*
    * Typically, here is the parent. Make sure this code is called
    * _after_ both subprocesses were launched!
    * Use wait() or waitpid() and wait until _both_ subprocesses are finished!
    * Then use exit()
    */
    else { /* parent */
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