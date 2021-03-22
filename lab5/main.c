#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    const int ARGS_NUM = 2;
    if(argc != ARGS_NUM) {
        perror("Wrong args num");
        return EXIT_FAILURE;
    }

    //const int TABLE_SIZE = 100;
    //off_t offsets[TABLE_SIZE] = {0};
    //size_t lineLength[TABLE_SIZE] = {0};

    const int FILE_OPEN_SUCCESS = 0;
    const int FILE_OPEN_FAIL = -1;
    const int FILE_ARG_NUM = 1;
    int fileDescriptor = FILE_OPEN_SUCCESS;
    fileDescriptor = open(argv[FILE_ARG_NUM], O_RDONLY);
    if(fileDescriptor == FILE_OPEN_FAIL){
        perror("Fail to open file");
        return EXIT_FAILURE;
    }

    size_t linesNum = 0;
    // ...


    const int FILE_CLOSE_FAIL = -1;
    int closeFileCheck = close(fileDescriptor);
    if(closeFileCheck == FILE_CLOSE_FAIL){
        perror("File close fail");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
