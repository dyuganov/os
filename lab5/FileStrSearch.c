#include "FileStrSearch.h"

int isCorrectLineNum(const size_t lineNumber, const size_t linesNum){
    return (lineNumber > 0) && (lineNumber <= TABLE_SIZE) && (lineNumber <= linesNum);
}

void initTable(void* table, const int value){
    memset(table, value, TABLE_SIZE);
}

int openFile(const char* fileName){
    int fileDescriptor = FILE_OPEN_FAIL;
    fileDescriptor = open(fileName, O_RDONLY);
    if(fileDescriptor == FILE_OPEN_FAIL){
        perror("Fail to open file");
    }
    return fileDescriptor;
}

int closeFile(int fileDescriptor){
    int closeFileCheck = close(fileDescriptor);
    if(closeFileCheck == FILE_CLOSE_FAIL){
        perror("File close fail");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

size_t createOffsetTable(off_t* offsets, size_t* lineLength, const int fileDescriptor){
    size_t linesNum = 0;
    size_t currLineLength = 0;
    char tmp = 0;
    ssize_t readResult = 0;
    long lseekResult = 0;

    offsets[0] = lseek(fileDescriptor, 0L, SEEK_CUR);
    while (NUM_NOT_NULL){
        readResult = read(fileDescriptor, &tmp, READ_CNT);
        if(readResult == READ_END) break;
        if(tmp == END_LINE_CHAR || tmp == EOF){
            ++currLineLength;
            lineLength[linesNum++] = currLineLength;
            lseekResult = lseek(fileDescriptor, 0L, SEEK_CUR);
            if(lseekResult == LSEEK_FAIL) {
                perror("Lseek fail");
                return 0;
            }
            offsets[linesNum] = lseekResult;
            currLineLength = 0;
        } else{
            ++currLineLength;
        }
        if(linesNum >= TABLE_SIZE){
            perror("File is too big");
            return 0;
        }
    }
    return linesNum;
}

size_t findLongestStrSize(const size_t* lineLength, const size_t linesNum){
    int max = 0;
    for (size_t i = 0; i < linesNum; ++i){
        if(lineLength[i] > max) max = lineLength[i];
    }
    return max;
}

int isStop(int lineNumber){
    return lineNumber == 0;
}

void printStringsToUser(const int fileDescriptor, const off_t* offsets, const size_t* lineLength, const size_t linesNum){
    if(linesNum < 1) return;
    const size_t strBufSize = findLongestStrSize(lineLength, linesNum)+1;
    int scanfResult = 0;
    int lineNumber = 0;
    while(1){
        printf("Enter string number\n");
        scanfResult = scanf("%d", &lineNumber);
        if(isStop(lineNumber)) return;
        if(scanfResult != 0 && isCorrectLineNum(lineNumber, linesNum)){
            char currStrBuf[strBufSize];
            lseek(fileDescriptor, offsets[lineNumber-1], SEEK_SET);
            read(fileDescriptor, currStrBuf, lineLength[lineNumber - 1]);
            printf("Your line: %s\n", currStrBuf);
        }
        else{
            printf("Wrong value\n");
            break;
        }
    }
}
