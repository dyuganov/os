#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <glob.h>
#include <stdbool.h>

#define MAX_PATTERN_SIZE (256)
#define NOT_END_OF_INPUT (1)
#define GLOB_SUCCESS (0)
#define ARGS_NUM (1)

/*bool isSlash(const char currentSymbol) {
    if (currentSymbol == '/') {
        fprintf(stderr, "Symbol '/' is not allowed\n");
        return true;
    }
    return false;
}*/

bool isStopSign(const char currentSymbol){
    if (currentSymbol == EOF || currentSymbol == '\n') {
        return true;
    }
    return false;
}

bool isShortPattern(const int patternLength){
    if (patternLength == 0) {
        fprintf(stderr, "Wrong input. The pattern can't be empty\n");
        return true;
    }
    return false;
}

bool isGlobError(const int globResult){
    if(globResult != GLOB_SUCCESS){
        perror("glob error");
        return true;
    }
    return false;
}

bool isWrongArgsNum(const int argc){
    if(argc != ARGS_NUM){
        fprintf(stderr, "Wrong args num\n");
        return true;
    }
    return false;
}

int readPattern(char* pattern) {
    int patternLength = 0;
    while (NOT_END_OF_INPUT) {
        char currentSymbol = (char)fgetc(stdin);
        if (isStopSign(currentSymbol)) break;
        //if (isSlash(currentSymbol)) return EXIT_FAILURE;
        pattern[patternLength] = currentSymbol;
        patternLength++;
    }
    if (isShortPattern(patternLength)) return EXIT_FAILURE;
    pattern[patternLength] = '\0';
    return EXIT_SUCCESS;
}

int printFileNames(char* pattern) {
    glob_t result;
    int globResult = glob(pattern, GLOB_NOCHECK, NULL, &result);
    if(isGlobError(globResult)) {
        globfree(&result);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < result.gl_pathc; i++) {
        printf("%s\n", result.gl_pathv[i]);
    }
    globfree(&result);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if(isWrongArgsNum(argc)) return 0;
    char pattern[MAX_PATTERN_SIZE];
    if (readPattern(pattern) == EXIT_FAILURE) return 0;
    if (printFileNames(pattern) == EXIT_FAILURE) return 0;
    return 0;
}

