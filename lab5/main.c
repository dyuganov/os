#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    const int ARGS_NUM = 2;
    if(argc != ARGS_NUM) {
        perror("Wrong args number.");
        return EXIT_FAILURE;
    }

    const int FILE_OPEN_SUCCESS = 0;
    const int FILE_OPEN_FAIL = -1;
    const int FILE_ARG_IDX = 1;
    int fileDescriptor = FILE_OPEN_SUCCESS;
    fileDescriptor = open(argv[FILE_ARG_IDX], O_RDONLY);
    if(fileDescriptor == FILE_OPEN_FAIL){
        perror("Fail to open file");
        return EXIT_FAILURE;
    }

    const size_t TABLE_SIZE = 100;
    off_t offsets[TABLE_SIZE];
    size_t lineLength[TABLE_SIZE];
    memset(offsets, 0, TABLE_SIZE);
    memset(lineLength, 0, TABLE_SIZE);

    size_t linesNum = 0;
    size_t currLineLen = 0;

    char tmp = 0;
    ssize_t readResult = 0;
    const size_t readCnt = 1;
    const size_t READ_END = 0;
    while (1){
        readResult = read(fileDescriptor, &tmp, readCnt);
        if(readResult == READ_END) break;
        if(tmp == '\n'){
            ++currLineLen;
            lineLength[linesNum] = currLineLen;
            offsets[linesNum] = lseek(fileDescriptor, 0L, SEEK_CUR);
            ++linesNum;
            currLineLen = 0;
        } else{
            ++currLineLen;
        }
    }


    const int FILE_CLOSE_FAIL = -1;
    int closeFileCheck = close(fileDescriptor);
    if(closeFileCheck == FILE_CLOSE_FAIL){
        perror("File close fail");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
