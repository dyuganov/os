#include "list.h"

#include <stdlib.h>
#include <string.h>

struct node {
    char* value;
    Node* next;
};

Node* initNode(){
    Node* head = (Node*)malloc(sizeof(Node));
    head->next = NULL;
    head->value = NULL;
    return head;
}

void freeNode(Node* currentNode){
    free(currentNode->value);
    free(currentNode);
}

Node* addString(char* newLine){
    Node* newNode = NULL;
    newNode = (Node*)malloc(sizeof(Node));
    newNode->value = (char*)malloc(strlen(newLine) + 1);
    strcpy(newNode->value, newLine);
    newNode->next = NULL;
    return(newNode);
}



