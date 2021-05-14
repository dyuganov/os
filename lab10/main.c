#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdbool.h>

#define FORK_ERROR (-1)
#define COMMAND_NAME_IDX (1)
#define CHILD_PROC_EXIT_ERROR (0)

bool isForkError(const int forkResult){
    if(forkResult == FORK_ERROR){
        perror("Fork error");
        return true;
    }
    return false;
}

bool isWrongArgsNum(const int argc){
    if(argc < 2) {
        fprintf(stderr, "Wrong args number\n");
        return true;
    }
    return false;
}

bool isProcFinishedWithSignal(const int status){
    if(WIFSIGNALED(status) != false){
        const int signalNum = WTERMSIG(status);
        fprintf(stderr, "Child process finished with signal: %d\n", signalNum);
        return true;
    }
    return false;
}

bool isChildProcFinished(const int status){
    if(WIFEXITED(status) == CHILD_PROC_EXIT_ERROR){
        fprintf(stderr, "Child process exit error\n");
        return false;
    }
    return true;
}



int main(int argc, char *argv[]) {
    if(isWrongArgsNum(argc)) return 0;
    int status;
    int forkResult = fork();
    if(isForkError(forkResult)) return 0;
	if (forkResult == 0) {
        execvp(argv[COMMAND_NAME_IDX], &argv[COMMAND_NAME_IDX]);
		perror(argv[1]);
		exit(127);
	}
    pid_t waitResult = 0;
	waitResult = wait(&status);
    if(isChildProcFinished(status) && !isProcFinishedWithSignal(status)) {
        int exitStatus = WEXITSTATUS(status);
        printf("Exit status: %d\n", exitStatus);
    }
    
    return 0;
}
