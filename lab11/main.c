#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define COMMAND_NAME_IDX (1)
#define EXECVP_ERROR (-1)

bool isExecvpError(const int execvpResult){
    if(execvpResult == EXECVP_ERROR){
        perror("Execvpe error");
        return true;
    }
    return false;
}

int execvpe(char *fileName, char *argv[], char *envp[]) {
    extern char** environ;
    char** old_environ = environ;
    environ = envp;

    int execvpResult = execvp(fileName, argv);
    if(isExecvpError(execvpResult)) {
        environ = old_environ;
    }
    return execvpResult;
}

bool isWrongArgsNum(const int argc){
    if(argc < 2) {
        fprintf(stderr, "Wrong args number\n");
        return true;
    }
    return false;
}

int main (int argc, char *argv[], char *envp[]){
    if(isWrongArgsNum(argc)) return 0;
    execvpe(argv[COMMAND_NAME_IDX], &argv[COMMAND_NAME_IDX], envp);
    return 0;
}