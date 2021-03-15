#include "list.h"

#include <stdlib.h>
#include <string.h>

#define NULL_POINTER_ARGUMENT (-1)
#define MALLOC_FAIL (-2)
#define LIST_CREATION_FAIL (-3)
#define NODE_CREATION_FAIL (-4)


struct node {
    char* value;
    Node* next;
};


Node* createNode(){
    Node* head = (Node*)malloc(sizeof(Node));
    head->next = NULL;
    head->value = NULL;
    return head;
}

void freeNode(Node* currentNode){
    free(currentNode->value);
    free(currentNode);
}

struct list{
    Node* head;
    Node* last;
};

List* createList(){
    List* newList = (List*)malloc(sizeof(List));
    newList->head = createNode();
    newList->last = newList->head;
}

char* createString(const unsigned int length){
    return (char*)malloc(length * sizeof(char));
}

void addNode(List* list, const char* newLine){
    Node* newNode = NULL;
    newNode = createNode();
    newNode->value = createString(strlen(newLine) + 1);
    strcpy(newNode->value, newLine);
    newNode->next = NULL;
    list->last->next = newNode;
    list->last = newNode;
}

int deleteList(List* list){
    if(list == NULL) return NULL_POINTER_ARGUMENT;
    Node* nextNode = NULL;
    Node* currentNode = list->head->next;
    while (currentNode != NULL) {
        nextNode = currentNode->next;
        freeNode(currentNode);
        currentNode = NULL;
        currentNode = nextNode;
    }
    free(list->head);
    free(list);
    return EXIT_SUCCESS;
}




