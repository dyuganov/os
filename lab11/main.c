#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define COMMAND_NAME_IDX (1)
#define EXECVP_ERROR (-1)
#define END_OF_ARGS (NULL)

extern char** environ;

bool isWrongArgsNum(const int argc){
    const int MIN_ARGS_NUM = 2;
    if(argc < MIN_ARGS_NUM) {
        fprintf(stderr, "Wrong args number\n");
        return true;
    }
    return false;
}

bool isExecvpError(const int execvpResult){
    if(execvpResult == EXECVP_ERROR){
        perror("Execvpe error");
        return true;
    }
    return false;
}

int execvpe(char *fileName, char *argv[], char *envp[]) {
    char** old_environ = environ;
    environ = envp;

    int execvpResult = execvp(fileName, argv);
    if(isExecvpError(execvpResult)) {
        environ = old_environ;
    }
    return execvpResult;
}

int main (int argc, char *argv[], char *envp[]){
    if(isWrongArgsNum(argc)) return 0;
    char *new_envp[2] = { "PATH=/home/students/19200/n.dyuganov/lab11/prog", END_OF_ARGS };
    execvpe(argv[COMMAND_NAME_IDX], &argv[COMMAND_NAME_IDX], new_envp);
    return 0;
}