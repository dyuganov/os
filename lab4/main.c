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
    char* fgetsResult = NULL;
    int errFlag = 0;
    while (1){
        fgetsResult = fgets(line, BUF_SIZE, stdin);
        if(fgetsResult == NULL){
            break;
        }
        if(line[0] == READ_STOP_SIGN){
            break;
        }
        int addStringResult = addString(list, line);
        if(addStringResult == EXIT_FAILURE){
            break;
        }
    }

    printf("Your lines: \n");
    printList(list);
    deleteList(list);
    return EXIT_SUCCESS;
}
