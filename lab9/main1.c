#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define ARGS_NUM (2)
#define END_OF_ARGS (NULL)
#define STATUS_IGNORE (NULL)
#define FORK_ERROR (-1)
#define WAIT_ERROR (-1)
#define CHILD_PROCESS (0)
#define EXECL_ERROR (-1)

bool isForkError(const int forkResult){
    if(forkResult == FORK_ERROR){
        perror("Fork error");
        return true;
    }
    return false;
}

bool isExeclError(const int execlResult){
    if(execlResult == EXECL_ERROR){
        perror("execl error");
        return true;
    }
    return false;
}

bool isWrongArgsNum(const int argc){
    if(argc != ARGS_NUM) {
        fprintf(stderr, "Wrong args number\n");
        return true;
    }
    return false;
}

bool isWaitError(const pid_t waitResult){
    if(waitResult == WAIT_ERROR) {
        perror("wait error");
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if(isWrongArgsNum(argc)) return 0;

    pid_t forkResult = fork();
    if(isForkError(forkResult)) return 0;
    if (forkResult == CHILD_PROCESS) {
        int execlResult = execl("/bin/cat", "cat", argv[1], END_OF_ARGS);
        if(isExeclError(execlResult)) return 0;
    }
    printf("Text written by parent\n");
    
    return 0;
}
