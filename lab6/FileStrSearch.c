#include "FileStrSearch.h"

int isCorrectLineNum(const size_t lineNumber, const size_t linesNum){
    return (lineNumber > 0) && (lineNumber <= TABLE_SIZE) && (lineNumber <= linesNum);
}

void initTable(void* table, const int value){
    memset(table, value, TABLE_SIZE);
}

bool isLseekError(const off_t lseekResult){
    if(lseekResult == LSEEK_FAIL) {
        perror("Lseek fail");
        return true;
    }
    return false;
}

bool isOpenError(const int fileDescriptor){
    if(fileDescriptor == FILE_OPEN_FAIL) {
        perror("Open file fail");
        return true;
    }
    return false;
}

bool isCloseError(const int closeResult){
    if(closeResult == FILE_CLOSE_FAIL) {
        perror("Close file fail");
        return true;
    }
    return false;
}

bool isReadError(const ssize_t readResult){
    if(readResult == READ_FAIL) {
        perror("Read fail");
        return true;
    }
    return false;
}

bool isReadEnd(const ssize_t readResult){
    if(readResult == READ_END) return true;
    return false;
}

int openFile(const char* fileName){
    int fileDescriptor = FILE_OPEN_FAIL;
    fileDescriptor = open(fileName, O_RDONLY);
    if(isOpenError(fileDescriptor)) return FILE_OPEN_FAIL;
    return fileDescriptor;
}

int closeFile(int fileDescriptor){
    int closeResult = close(fileDescriptor);
    if(isCloseError(closeResult)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

bool isLineEnd(const char symbol){
    if(symbol == END_LINE_CHAR || symbol == EOF) return true;
    return false;
}

bool isBigFile(const size_t linesNum){
    if(linesNum >= TABLE_SIZE) {
        fprintf(stderr, "File is too big. Table size is %d\n", TABLE_SIZE);
        return true;
    }
    return false;
}

size_t createOffsetTable(off_t* offsets, size_t* lineLength, const int fileDescriptor){
    size_t linesNum = 0;
    size_t currLineLength = 0;
    char currChar = 0;
    offsets[0] = 0;
    while (NUM_NOT_NULL){
        ssize_t readResult = read(fileDescriptor, &currChar, READ_CNT);
        if(isReadError(readResult)) return NO_LINES;
        if(isReadEnd(readResult)) break;
        if(isLineEnd(currChar)){
            ++currLineLength;
            lineLength[linesNum] = currLineLength;
            ++linesNum;
            long lseekResult = lseek(fileDescriptor, 0L, SEEK_CUR);
            if(isLseekError(lseekResult)) return NO_LINES;
            offsets[linesNum] = lseekResult;
            currLineLength = 0;
        } else{
            ++currLineLength;
        }
        if(isBigFile(linesNum)) return NO_LINES;
    }
    return linesNum;
}

size_t findLongestStrSize(const size_t* lineLength, const size_t linesNum){
    long long max = 0;
    for (size_t i = 0; i < linesNum; ++i){
        if(lineLength[i] > max) max = lineLength[i];
    }
    return max;
}

bool isStop(const long long lineNumber){
    return (lineNumber == 0);
}

bool isSelectError(const int selectResult){
    if(selectResult == SELECT_ERROR){
        perror("Select error");
        return true;
    }
    return false;
}

bool isDigit(char symbol){
    if(symbol >= '0' && symbol <= '9') return true;
    return false;
}

bool isNumber(const char* number, const size_t size){
    for(size_t i = 0; i < size; ++i){
        if(!isDigit(number[i])) return false;
    }
    return true;
}

bool isFgetsError(const char* fgetsResult){
    if(fgetsResult == NULL) {
        fprintf(stderr, "Fgets error\n");
        return true;
    }
    return false;
}

long long getLineNum(){
    long long result = -1;
    char lineNumber[LINE_NUM_SIZE];
    char* fgetsResult = fgets(lineNumber, LINE_NUM_SIZE, stdin);
    if(isFgetsError(fgetsResult)) return result;

    char* endPtr = NULL;
    size_t linesNumLength = strnlen(lineNumber, LINE_NUM_SIZE) - 1;
    if(isNumber(lineNumber, linesNumLength)) result = strtoll(lineNumber, &endPtr, LLINT);
    return result;
}

int waitForInput(){
    fd_set readDescriptors;
    FD_ZERO(&readDescriptors);
    FD_SET(STDIN_FILENO, &readDescriptors);
    int selectResult = -1;
    struct timeval timeout;
    timeout.tv_sec = TIME_SEC;
    timeout.tv_usec = TIME_USEC;
    if(FD_ISSET(STDIN_FILENO, &readDescriptors)) {
        selectResult = select(STDIN_FILENO + 1, &readDescriptors, NULL, NULL, &timeout);
    }
    if(isSelectError(selectResult)) return SELECT_FAIL;
    if(selectResult == 0) return TIME_OVER;
    if(FD_ISSET(STDIN_FILENO, &readDescriptors)) return TIME_NOT_OVER;
}

bool isPrintFileError(int printFileResult){
    if(printFileResult == EXIT_FAILURE) {
        fprintf(stderr, "Print file error\n");
        return true;
    }
    return false;
}

int printFile(const int fileDescriptor){
    char currChar = 0;
    int lseekResult = lseek(fileDescriptor, 0L, SEEK_SET);
    if(isLseekError(lseekResult)) return EXIT_FAILURE;
    while (NOT_FILE_END){
        ssize_t readResult = read(fileDescriptor, &currChar, READ_CNT);
        if(isReadError(readResult)) return EXIT_FAILURE;
        if(isReadEnd(readResult)) break;
        printf("%c", currChar);
    }
    return EXIT_SUCCESS;
}

void printStringsToUser(const int fileDescriptor, const off_t* offsets, const size_t* lineLength, const size_t linesNum){
    if(linesNum < 1) return;
    const size_t strBufSize = findLongestStrSize(lineLength, linesNum);
    char currStrBuf[strBufSize];
    while(1){
        int waitResult = waitForInput();
        if(waitResult == TIME_OVER){
            isPrintFileError(printFile(fileDescriptor));
            return;
        }
        if(waitResult == SELECT_FAIL){
            return;
        }
        long long lineNumber = getLineNum();
        if(isStop(lineNumber)) return;
        if(isCorrectLineNum(lineNumber, linesNum)){
            memset(currStrBuf, 0, strBufSize);
            int lseekResult = lseek(fileDescriptor, offsets[lineNumber-1], SEEK_SET);
            if(isLseekError(lseekResult)) return;
            int readResult = read(fileDescriptor, currStrBuf, lineLength[lineNumber - 1]);
            if(isReadError(readResult)) return;
            printf("%s", currStrBuf);
        }
        else{
            fprintf(stderr, "Wrong value. Max line num is %u\n", linesNum);
        }
    }
}
