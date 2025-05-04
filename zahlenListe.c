//
// Created by root on 5/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#define MAX_LINE 1024

typedef struct Number {
    int value;
    struct Number *next;
} Number;

Number *head = NULL;  // globale Liste, für Signalhandler nötig

// Funktion zum Freigeben der Liste
void cleanup_memory() {
    printf("\n==================\nCleaning up memory\n==================\n");

    Number *current = head;
    while (current) {
        Number *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    exit(0);  // wichtig für sauberes Beenden nach Signal
}

// Prüft, ob der String eine gültige positive Zahl ist
int is_valid_number(const char *str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

// Prüft, ob Zahl schon in Liste ist
int exists_in_list(int value) {
    Number *current = head;
    while (current) {
        if (current->value == value) return 1;
        current = current->next;
    }
    return 0;
}

// Fügt eine neue Zahl zur Liste hinzu (am Ende)
void add_number(int value) {
    if (exists_in_list(value)) return;  // doppelte ignorieren

    Number *new_node = malloc(sizeof(Number));
    if (!new_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->next = NULL;

    if (!head) {
        head = new_node;
    } else {
        Number *current = head;
        while (current->next) current = current->next;
        current->next = new_node;
    }
}

// Tauscht zwei Knoten (nicht Werte!)
void swap_nodes(Number **a, Number **b) {
    Number *temp = *a;
    *a = *b;
    *b = temp;
}

// Pointer-basierter Bubblesort: vertauscht Knoten, nicht Werte
void sort_list() {
    if (!head || !head->next) return;

    int swapped;
    do {
        swapped = 0;
        Number **ptr = &head;

        while ((*ptr)->next) {
            Number *curr = *ptr;
            Number *next = curr->next;

            if (curr->value > next->value) {
                curr->next = next->next;
                next->next = curr;
                *ptr = next;
                swapped = 1;
            }

            ptr = &(*ptr)->next;
        }
    } while (swapped);
}

// Gibt alle Zahlen aus
void print_list() {
    Number *current = head;
    while (current) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}



int main() {
    char line[MAX_LINE];

    // Speicherfreigabe bei STRG+C registrieren
    signal(SIGINT, cleanup_memory);

    // Optional auch bei normalem Programmende freigeben
    atexit(cleanup_memory);

    //printf("$ ./zahlenliste\n");

    while (1) {
        printf("Eingabe: ");
        if (!fgets(line, sizeof(line), stdin)) {
            break; // EOF z.B. bei STRG+D
        }

        char *token = strtok(line, " \t\n");
        while (token) {
            if (is_valid_number(token)) {
                int num = atoi(token);
                if (num > 0) {
                    add_number(num);
                }
            }
            token = strtok(NULL, " \t\n");
        }

        sort_list();

        printf("Ausgabe: ");
        print_list();
    }

    return 0;
}
