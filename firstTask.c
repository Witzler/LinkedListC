//
// Created by root on 4/15/25.
//
#include <stdio.h>
#include <signal.h>  // For Signalhandling
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // für isdigit()
#include <stdbool.h>


#define input_Size 1024

typedef struct node {
    int data;
    struct node* next;
    struct node* previous;
} Node;

Node *head = NULL;  // global list necessary for Signalhandler
Node *tail = NULL; //= NULL

Node * createNewNode (int payload) {
    Node* newNode;
    if ((newNode = (Node*)malloc(sizeof(Node))) == NULL) {
        printf("Fehler bei der Speicherzuweisung!\n");
        exit(1);
    }
    if (head == NULL) { //head == NULL or !head
        head = newNode;
        head -> data = payload;
        //tail = newNode; happens after this function already
    } else {
        tail -> next = newNode;

        newNode->data = payload;
        newNode->next = NULL;
        newNode->previous = tail;
    }
    return newNode;
}

int isNumber(const char *str) {
    while (*str != '\0') {
        if (!isdigit(*str)) {
            return 0;  // Nicht-Ziffer gefunden
        }
        str++;
    }
    return 1;  // Alle Zeichen sind Ziffern
}

int isNotDoubleData(const char *str) {
    Node *current = head;

    while (current!= NULL) {
        if (current->data == atoi(str)) {
            return 0;
        }
        current = current->next;
    }

    return 1;
}

void swap( Node *a,  Node *b){
    int temp=a->data;
    a->data=b->data;
    b->data=temp;
}
void bubblesortList (void) {
    if (head == NULL||head -> data == 0|| tail == NULL||tail -> data == 0) {
        printf("Head/Tail wasn't initialized to sort.\n");return;
    }
    bool swapped;
    Node *ptr;
    Node *lastSorted = NULL;
    do {
        swapped = false;
        ptr = head;

        while (ptr->next != lastSorted) {
            if (ptr->data > ptr->next->data) {
                swap(ptr, ptr->next);
                swapped = true;
            }
            ptr = ptr->next;
        }
        lastSorted = ptr;
    } while (swapped);

}

/*
*void swap(struct Number *a, struct Number *b){
int temp=a->value;
a->value=b->value;
b->value=temp;
}
*/

void printList(void) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

}

void cleanup_memory() {  //void cleanup_memory(void) vs void cleanup_memory() ?
    // Speicherbereinigung hier
}

int main(void) {

    printf("Hallo, Welt lol!\n");

    char input[input_Size];
    signal(SIGINT, cleanup_memory); //=Signal Interrupt
    int run = 1;

    while (run){
        printf("Eingabe: ");
        fgets(input, sizeof(input), stdin); //Read User Input
        input[strcspn(input, "\n")] = 0; //Removes the "\n" added by fgets from the input

        char *token = strtok(input, " ");  // Trenne bei Leerzeichen
        // Solange es noch Teile gibt, mach weiter
        while (token != NULL) {
            if (isNumber(token) && isNotDoubleData(token)) { //
                int number = atoi(token);
                tail = createNewNode(number);
                //printf("Teil: %d\n", number);  // Verarbeite den Teil
            }
            token = strtok(NULL, " ");  // Nächstes Token
        }

        bubblesortList();
        printf("Ausgabe: ");
        printList();

    }

    return 0;
}
