#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    const int BUF_SIZE = 100;
    char line[BUF_SIZE];
    List* list = createList();
    if(list == NULL){
      perror("Can't create list");
      return EXIT_FAILURE;
    }

    printf("Enter lines. Put '.' to the line beginning to exit. \n");
    const char READ_STOP_SIGN = '.';
    while (fgets(line, BUF_SIZE, stdin)){
        if(line[0] == READ_STOP_SIGN){
            break;
        }
        int addStringResult = addString(list, line);
        if(addStringResult == MALLOC_FAIL){
            perror("List addString error");
            deleteList(list);
            return EXIT_FAILURE;
        } else if(addStringResult == NULL_POINTER_ARGUMENT){
            deleteList(list);
            return EXIT_FAILURE;
        }
    }

    printf("Your lines: \n");
    printList(list);
    deleteList(list);
    return EXIT_SUCCESS;
}
