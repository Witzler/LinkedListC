//
// Created by root on 4/15/25.
//
#include <stdio.h>
#include <signal.h>  // For Signalhandling
#include <string.h>
#include <stdlib.h> //malloc
#include <ctype.h>  // für isdigit()
#include <stdbool.h>


#define input_Size 1024

typedef struct node {
    int data;
    struct node* next;
    struct node* previous;
} Node;

Node *head = NULL;  //global list necessary for Signalhandler
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
            return 0;
        }
        str++;
    }
    return 1;  // All chars are digits
}

int isNotDoubleData(const char *str) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == atoi(str)) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void swap(Node *a, Node *b) {
    if (a == b) {return;}

    Node *ap = a->previous;
    Node *bn = b->next;

    if (a->next != b) {
        //If not neighboured they get swapped in the list

        if (ap != NULL) {ap->next = b;}
        if (bn != NULL) {bn->previous = a;}

        b->previous = ap;
        a->next = bn;

        //Temporary Links
        //Node *temp = b->next;
        b->next = a->next;
        a->previous = b->previous;

        b->next = a;
        a->previous = b;
    } else { //They are neighbours
        if (ap != NULL) ap->next = b;
        if (bn != NULL) bn->previous = a;

        b->previous = ap;
        a->next = bn;

        b->next = a;
        a->previous = b;
    }

    //Update Head/Tail if necessary
    if (head == a){ head = b;}
    else if (head == b) {head = a;}
    if (tail == b) {tail = a;}
    else if (tail == a) {tail = b;}
}

void bubblesortList(void) {
    if (head == NULL) return;

    bool swapped;
    do {
        swapped = false;
        Node *current = head;

        while (current && current->next) {
            if (current->data > current->next->data) {
                swap(current, current->next);
                swapped = true;
                if (current->previous) current = current->previous;
            } else {
                current = current->next;
            }
        }
    } while (swapped);
}

/*void swap( Node *a,  Node *b){
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
}*/

void printList(void) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void cleanup_memory() {  //void cleanup_memory(void) vs. void cleanup_memory() ?

    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    printf("\n==================\nCleaning up memory\n==================\n");
    exit(0);
}

int main(void) {
    //printf("Hallo, Welt lol!\n");

    char input[input_Size];
    int run = 1;

    signal(SIGINT, cleanup_memory); //=Signal Interrupt
    //atexit(cleanup_memory);      //For return 0 Interrupt


    while (run){
        printf("Eingabe: ");
        fgets(input, sizeof(input), stdin); //Read User Input
        input[strcspn(input, "\n")] = 0; //Removes the "\n" added by fgets from the input

        char *token = strtok(input, " ");  // Seperate with " " as delimiter
        // Solange es noch Teile gibt, mach weiter
        while (token != NULL) {
            if (isNumber(token) && isNotDoubleData(token)) {
                int number = atoi(token);
                tail = createNewNode(number);
                //printf("Teil: %d\n", number);  // One Token
            }
            token = strtok(NULL, " ");  //Next Token
        }

        bubblesortList();
        printf("Ausgabe: ");
        printList();
    }
    return 0;
}
