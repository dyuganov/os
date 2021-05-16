#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <siginfo.h>
#include <stdio.h>

#define FORK_ERROR -1
#define CHILD_PROCESS 0
#define WAIT_ERROR -1
#define WAIT_STAT_ERROR 0
#define ZERO_RETURN 0

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Enter the name of a program that you want to execute.\n");
		return EXIT_FAILURE;
	}
	
	pid_t forkReturn = fork();
	int waitStatus = 0;
	if (forkReturn == (pid_t)FORK_ERROR){
		perror("fork error");
		return EXIT_FAILURE;
	}
	
	if (forkReturn == CHILD_PROCESS){
		execvp(argv[1], &argv[1]);
		perror("exec error");
		return EXIT_FAILURE;
	}
	
	printf ("The process ID of the child process: %d\n", forkReturn);
	pid_t waitReturn = wait(&waitStatus);
	
	if (waitReturn == WAIT_ERROR){
		perror("wait error");
		return EXIT_FAILURE;
	}
	
	if (WIFEXITED(waitStatus) != ZERO_RETURN){
		printf("Child process stopped by exit or returned this code from main, code: %d\n", WEXITSTATUS(waitStatus));
		return EXIT_SUCCESS;
	}
	
	if (WIFSIGNALED(waitStatus) != ZERO_RETURN){
		//printf("Child exited because a signal was not caught, the signal number: %d\n", WTERMSIG(waitStatus));
		printf("Child exited because a signal was not caught.\n");
		psignal(WTERMSIG(waitStatus), "The signal number: ");
		if (WCOREDUMP(waitStatus) != ZERO_RETURN){
			printf("The core file was created.\n");
		}
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
