#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define CHILD_PROCESS 0
#define FORK_ERROR -1
#define EXEC_ERROR -1

int main(int argc, char **argv) {
	pid_t forkResult, waitResult;
	int status, error;
	if(argc < 2) {
		fprintf(stderr, "No arguments\n");
		return EXIT_FAILURE;
	}
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		error = execvp(argv[1], &argv[1]);
		if(error == EXEC_ERROR) {
			perror("exec error");
			return EXIT_FAILURE;
		}
	}
	printf("Parent: waiting for child process with pid=%ld\n\n", forkResult);
	waitResult = wait(&status);
	printf("\nParent: child's status is %d\n", WEXITSTATUS(status));
	return EXIT_SUCCESS;
}
