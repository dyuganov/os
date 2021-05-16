#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define FORK_ERROR -1
#define CHILD_PROCESS 0
#define WAIT_ERROR -1
#define ZERO_RETURN 0

int main(int argc, char **argv){
	pid_t forkReturn = fork();
	int waitStatus;
	if (forkReturn == (pid_t)FORK_ERROR){
		perror("fork error");
		return EXIT_FAILURE;
	}
	
	if (forkReturn == CHILD_PROCESS){
		execl("/bin/cat", "cat", "file.c", (char *)0);
		perror("exec error");
		return EXIT_FAILURE;
	}
	
	pid_t waitReturn = wait(&waitStatus);
	
	if (waitReturn == WAIT_ERROR){
		perror("wait error");
		return EXIT_FAILURE;
	}
	
	if (WIFEXITED(waitStatus) == ZERO_RETURN){
		perror("child process error");
	}
	
	int printfReturn = printf("I'm a parent.\n");
	if (printfReturn < 0){
		perror("printf error");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
