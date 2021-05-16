#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define CHILD_PROCESS 0
#define ARGV_END NULL
#define FORK_ERROR -1
#define EXEC_ERROR -1

int main(int argc, char **argv) {
	pid_t forkResult, waitResult;
	int status, i, error;
	char *arg0;
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
		arg0 = argv[0];
		argv[0] = "cat";
		error = execvp("cat", argv);
		if(error == EXEC_ERROR) {
			argv[0] = arg0;
			perror("Exec error");
			return EXIT_FAILURE;
		}
	}
	printf("Parent: waiting for child process with pid=%ld\n\n", forkResult);
	waitResult = wait(&status);
	printf("\nParent: child's status is %d\n", WEXITSTATUS(status));
	return EXIT_SUCCESS;
}
