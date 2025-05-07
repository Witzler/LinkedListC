//
// Created by root on 5/6/25.
//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

/*
gcc -std=c99 -Wall -pedantic -o redirect redirect.c
./redirect /tmp/test1.out - ps -aux
./redirect /tmp/test2.out /tmp/test2.err cp /etc/hosts /etc/foo /tmp
 */

int main(int argc, char *argv[]) {

    char **arguments= malloc((argc-2) * sizeof(char *) );
    int y = 0;
    for(int i = 3; i<=argc;i++){
        arguments[y] = argv[i];
        y++;
    }

    int output;
    int error;

    if (strcmp(argv[1], "-") == 0) { //output
        dup2(STDOUT_FILENO, STDOUT_FILENO);
    } else {
        if ((output = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            perror("Error opening output file");
            exit(1);
        }
        dup2(output, STDOUT_FILENO);
        close(output);
    }

    if (strcmp(argv[2], "-") == 0) { //error
        dup2(STDERR_FILENO, STDERR_FILENO);
    } else {
        if ((error = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            perror("Error opening error file");
            exit(1);
        }
        dup2(error, STDERR_FILENO);
        close(error);
    }

    //execvp(arguments[0], arguments);
    if (execvp(arguments[0], arguments) == -1) {
        perror("execvp failed");
        exit(1);
    }

    return 0;
}