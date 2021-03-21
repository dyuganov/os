#include "list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct node {
    char* value;
    Node* next;
};

struct list{
    Node* head;
    Node* last;
};

Node* createNode(){
    Node* head = (Node*)malloc(sizeof(Node));
    if(head == NULL){
        perror("Node creation error");
    }
    head->next = NULL;
    head->value = NULL;
    return head;
}

void freeNode(Node* currentNode){
    free(currentNode->value);
    free(currentNode);
}

List* createList(){
    List* newList = (List*)malloc(sizeof(List));
    if(newList == NULL){
        perror("List creation error");
    }
    newList->head = NULL;
    newList->last = NULL;
    return newList;
}

char* createString(unsigned int length){
    char* newString = (char*)malloc(length * sizeof(char));
    if(newString == NULL){
        perror("String creation error");
    }
}

int addString(List* list, const char* newLine){
    if(list == NULL) {
        perror("AddString error. List ptr is NULL");
        return NULL_POINTER_ARGUMENT;
    }
    Node* newNode = NULL;
    newNode = createNode();
    if(newNode == NULL) {
        perror("Node creation error");
        return MALLOC_FAIL;
    }
    newNode->value = createString(strlen(newLine) + 1);
    if(newNode == NULL) {
        perror("AddNode error");
        return MALLOC_FAIL;
    }
    strcpy(newNode->value, newLine);
    newNode->next = NULL;
    if(list->head == NULL){
        list->head = newNode;
        list->last = newNode;
    }
    else{
        list->last->next = newNode;
        list->last = newNode;
    }
    return EXIT_SUCCESS;
}

void deleteList(List* list){
    if(list == NULL) return;
    Node* nextNode = NULL;
    Node* currentNode = list->head->next;
    while (currentNode != NULL) {
        nextNode = currentNode->next;
        freeNode(currentNode);
        currentNode = nextNode;
    }
    free(list->head);
    free(list);
}

void printList(List* list){
    if(list == NULL) return;
    Node* currNode = list->head;
    while (currNode != NULL){
        printf("%s\n", currNode->value);
        currNode = currNode->next;
    }
}
