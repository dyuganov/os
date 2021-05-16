#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

void printEnvp() {
	extern char **environ;
	int i = 0;
	for(; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
}

int main() {
	printEnvp();
	return EXIT_SUCCESS;
}
