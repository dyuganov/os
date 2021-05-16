#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdbool.h>

#define FORK_ERROR (-1)
#define COMMAND_NAME_IDX (1)
#define WAIT_ERROR (-1)
#define CHILD_PROCESS (0)
#define EXECVP_ERROR (-1)

bool isForkError(const pid_t forkResult){
    if(forkResult == FORK_ERROR){
        perror("Fork error");
        return true;
    }
    return false;
}

bool isWrongArgsNum(const int argc){
    const int MIN_ARGS_NUM = 2;
    if(argc < MIN_ARGS_NUM) {
        fprintf(stderr, "Wrong args number\n");
        return true;
    }
    return false;
}

bool isChildProcFinished(const int status){
    return WIFEXITED(status);
}

void printExitStatus(const int status){
    int exitStatus = WEXITSTATUS(status);
    fprintf(stdout,"Exit status: %d\n", exitStatus);
}

bool isExecvpError(const int execvpResult){
    if(execvpResult == EXECVP_ERROR){
        perror("execvp error");
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

bool isSignalInterrupted(const int status){
    return (WIFSIGNALED(status));
}

void printSignalNum(const int status){
    int signalNumber = WTERMSIG(status);
    fprintf(stderr, "Terminated by signal number: %d\n", signalNumber);
    if(WCOREDUMP(status)) fprintf(stderr, "Core-file was created.\n");
}

int main(int argc, char *argv[]) {
    if(isWrongArgsNum(argc)) return 0;
    int status = 0, execvpResult = 0;

    pid_t forkResult = fork();
    if(isForkError(forkResult)) return 0;
	if (forkResult == CHILD_PROCESS) {
        execvpResult = execvp(argv[COMMAND_NAME_IDX], &argv[COMMAND_NAME_IDX]);
        if(isExecvpError(execvpResult)) return 0;
	}
    pid_t waitResult = wait(&status);
	if(isWaitError(waitResult)) return 0;
    if(isChildProcFinished(status)) printExitStatus(status);
    if(isSignalInterrupted(status)) printSignalNum(status);

    return 0;
}
