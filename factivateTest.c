//
// Created by root on 5/6/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <PROG1> [ARGS...] <DELIMITER> <PROG2> [ARGS...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Delimiter suchen (z. B. ";")
    int delimiter_index = -1;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], ";") == 0) {
            delimiter_index = i;
            break;
        }
    }

    if (delimiter_index == -1 || delimiter_index == 1 || delimiter_index == argc - 1) {
        fprintf(stderr, "Error: delimiter missing or in wrong position.\n");
        exit(EXIT_FAILURE);
    }

    // Erste Befehlsliste vorbereiten
    char **prog1_argv = malloc((delimiter_index) * sizeof(char *));
    for (int i = 0; i < delimiter_index; ++i) {
        prog1_argv[i] = argv[i + 1]; // argv[1..delimiter_index-1]
    }
    prog1_argv[delimiter_index - 1 + 1] = NULL; // NULL-terminiert

    // Zweite Befehlsliste vorbereiten
    int prog2_argc = argc - delimiter_index - 1;
    char **prog2_argv = malloc((prog2_argc + 1) * sizeof(char *));
    for (int i = 0; i < prog2_argc; ++i) {
        prog2_argv[i] = argv[delimiter_index + 1 + i];
    }
    prog2_argv[prog2_argc] = NULL;

    // Kindprozess 1
    pid_t pid1 = fork();
    if (pid1 == 0) {
        execvp(prog1_argv[0], prog1_argv);
        perror("execvp failed for PROGRAM 1");
        exit(EXIT_FAILURE);
    } else if (pid1 < 0) {
        perror("fork failed for PROGRAM 1");
        exit(EXIT_FAILURE);
    }

    // Kindprozess 2
    pid_t pid2 = fork();
    if (pid2 == 0) {
        execvp(prog2_argv[0], prog2_argv);
        perror("execvp failed for PROGRAM 2");
        exit(EXIT_FAILURE);
    } else if (pid2 < 0) {
        perror("fork failed for PROGRAM 2");
        exit(EXIT_FAILURE);
    }

    // Elternprozess wartet auf beide Kinder
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    // Ressourcen aufräumen
    free(prog1_argv);
    free(prog2_argv);

    return 0;
}
