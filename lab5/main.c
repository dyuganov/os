#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define FILE_OPEN_SUCCESS 0
#define FILE_OPEN_FAIL -1
#define TABLE_SIZE 100
#define READ_END 0
#define END_LINE_CHAR '\n'
#define LSEEK_FAIL (off_t)-1



int isCorrectLineNum(int lineNum){
    return lineNum > 0 && lineNum <= TABLE_SIZE;
}

int main(int argc, char *argv[]) {
    const int ARGS_NUM = 2;
    if(argc != ARGS_NUM) {
        perror("Wrong args number.");
        return EXIT_FAILURE;
    }

    const int FILE_ARG_IDX = 1;
    int fileDescriptor = FILE_OPEN_SUCCESS;
    char* fileName = argv[FILE_ARG_IDX];
    fileDescriptor = open(fileName, O_RDONLY);
    if(fileDescriptor == FILE_OPEN_FAIL){
        perror("Fail to open file");
        return EXIT_FAILURE;
    }

    //init table
    off_t offsets[TABLE_SIZE];
    size_t lineLength[TABLE_SIZE];
    memset(offsets, 0, TABLE_SIZE);
    memset(lineLength, 0, TABLE_SIZE);

    // create offsets and length table
    size_t linesNum = 0;
    size_t currLineLength = 0;
    char tmp = 0;
    ssize_t readResult = 0;
    long lseekResult = 0;
    const size_t readCnt = 1;
    while (1){
        readResult = read(fileDescriptor, &tmp, readCnt);
        if(readResult == READ_END) break;
        if(tmp == END_LINE_CHAR){
            ++currLineLength;
            if(linesNum < TABLE_SIZE){
                perror("File is too big");
                return EXIT_FAILURE;
            }
            lineLength[linesNum] = currLineLength;
            lseekResult = lseek(fileDescriptor, 0L, SEEK_CUR);
            if(lseekResult == LSEEK_FAIL) {
                perror("Lseek fail");
                return EXIT_FAILURE;
            }
            offsets[linesNum] = lseekResult;
            ++linesNum;
            currLineLength = 0;
        } else{
            ++currLineLength;
        }
    }

    //getting str N and print it
    int scanfResult = 0;
    int lineNumber;
    printf("Enter string number\n");
    scanf("%s", &lineNumber); // add notCharCheck

    while(lineNumber != 0){
        if(isCorrectLineNum(linesNum)){

            printf("Enter string number\n");
            scanf("%d", &lineNumber);
        }
        else{
            break;
        }
    }

    // close file
    const int FILE_CLOSE_FAIL = -1;
    int closeFileCheck = close(fileDescriptor);
    if(closeFileCheck == FILE_CLOSE_FAIL){
        perror("File close fail");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}


