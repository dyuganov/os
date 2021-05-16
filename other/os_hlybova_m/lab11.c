#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/param.h>

#define ENVP_ELEMENTS_AMOUNT 5
#define LENGTH_ENVP_STRING 255
#define PATH_MAX MAXPATHLEN
#define FALSE 0
#define TRUE 1
#define OPEN_ERROR -1

extern char **environ;

int execvpe(const char *file, char *const argv[], char *envp[]){
	char *PATH = getenv("PATH");
	if (PATH == NULL){
		fprintf(stderr, "There's no PATH.\n");
		return EXIT_FAILURE;
	}
	char pathToFile[PATH_MAX];
	char *realpathReturn = realpath(file, pathToFile);
	if (realpathReturn == NULL){
		perror("realpath error");
		return EXIT_FAILURE;
	}
	environ = envp;
    execv(pathToFile, argv);
    perror("Error exec");
    return EXIT_FAILURE;
}

void *checkCalloc(size_t numbElem, size_t elSize){
	void *callocPtr = calloc(numbElem, elSize);
	if (callocPtr == NULL){
		perror("Calloc error");
	}
	return callocPtr;
}

void deleteEnvp(char **envp){
	int indexEnvp;
	for (indexEnvp = 0; indexEnvp < ENVP_ELEMENTS_AMOUNT; indexEnvp++){
		free(envp[indexEnvp]);
	}
	free(envp);
}

char **makeEnvp(){
	int indexEnvp;
	char **envp;
	envp = (char**)checkCalloc(ENVP_ELEMENTS_AMOUNT, sizeof(char));
	if (envp == NULL){
		return NULL;
	}
	for (indexEnvp = 0; indexEnvp < ENVP_ELEMENTS_AMOUNT; indexEnvp++){
		envp[indexEnvp] = (char*)checkCalloc(LENGTH_ENVP_STRING, sizeof(char));
		if (envp[indexEnvp] == NULL){
			deleteEnvp(envp);
			return NULL;
		}
	}
	envp[0] = "FIRST=I'M FIRST";
	envp[1] = "NUMBER=11";
	envp[2] = "NEWENVP=TRUE";
	envp[3] = "NAME=VALUE";
	envp[4] = NULL;
	return envp;
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Enter the name of a program that you want to execute.\n");
		return EXIT_FAILURE;
	}
	char **envp = makeEnvp();
	if (envp == NULL){
		return EXIT_FAILURE;
	}
	execvpe(argv[1], &argv[1], envp);
	fprintf(stderr, "execvpe error\n");
	return EXIT_FAILURE;
}
