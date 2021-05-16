#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define CHILD_PROCESS 0
#define ARGV_END NULL
#define FORK_ERROR -1

int main(int argc, char **argv) {
	pid_t forkResult, waitResult;
	int status;
	long long i;
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		execlp("cat", "cat", "/home/students/17200/golikov/OS_Labs/91011Lab/9lab.c", ARGV_END);
	}
	printf("Parent: waiting for child process with pid=%ld\n\n", forkResult);
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		sleep(0.1);
		execlp("ps", "ps", "-a", ARGV_END);
	}
	sleep(1);
	waitResult = wait(&status);
	printf("\nParent: child's status is %d\n", WEXITSTATUS(status));
	return EXIT_SUCCESS;
}
