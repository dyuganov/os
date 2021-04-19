#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "FileStrSearch.h"

int main(int argc, char *argv[]) {
    const int ARGS_NUM = 2;
    if(argc != ARGS_NUM) {
        perror("Wrong args number.");
        return EXIT_FAILURE;
    }

    const int fileDescriptor = openFile(argv[FILE_ARG_IDX]);
    if(fileDescriptor == FILE_OPEN_FAIL) return EXIT_FAILURE;

    off_t offsets[TABLE_SIZE];
    size_t lineLength[TABLE_SIZE];
    initTable(offsets, TABLE_STANDART_VAL);
    initTable(lineLength, TABLE_STANDART_VAL);

    size_t linesNum = createOffsetTable(offsets, lineLength, fileDescriptor);

    printStringsToUser(fileDescriptor, offsets, lineLength, linesNum);

    int closeResult = closeFile(fileDescriptor);
    if(closeResult == EXIT_FAILURE) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
