#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define CHILD_PROCESS 0
#define FORK_ERROR -1
#define EXEC_ERROR -1

/*
В данной программе используются такие же системные вызовы, что и в 10 лабораторной работе.
*/

extern char **environ;

/*
Функция execvpe

Аргументы: путь до файла программы, массив аргументов этой программы, массив переменных окружения

Описание работы функции:
Сохраняем массив переменных окружения, записываем во внешнюю переменную environ переданный массив envp, вызываем execvp() с переданными аргументами, при ошибке восстанавливаем значение переменной environ и возвращаем ошибку.
*/

int execvpe(char *file, char *argv[], char *envp[]) {
	int error;
	char **oldenvp = environ;
	environ = envp;
	error = execvp(file, argv);
	environ = oldenvp;
	return error;
}

/*
Функция для печати переменных окружения

Описание работы функции:
В цикле пока environ[i] != NULL (так как в конце массива environ обязательно должен стоять NULL) выводим значение данной переменной окружения.
*/

void printEnvp() {
	int i = 0;
	for(; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
}

char *env1[] = { "There", "is", "an", "other", "environ", NULL };

int main(int argc, char **argv) {
	pid_t forkResult;
	int status, error;
	if(argc < 2) {
		fprintf(stderr, "No arguments\n");
		return EXIT_FAILURE;
	}
	printf("Parent: \n");
	printEnvp();
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		error = execvpe(argv[1], &argv[1], env1);
		if(error == EXEC_ERROR) {
			perror("exec error");
			return EXIT_FAILURE;
		}
	}
	printf("Parent: waiting for child process with pid=%ld\n\n", forkResult);
	error = wait(&status);
	if(error == WAIT_ERROR) {
		perror("Wait error");
		return EXIT_FAILURE;
	}
	if(WIFEXITED(status)) printf("\nParent: child's status is %d\n", WEXITSTATUS(status));
	return EXIT_SUCCESS;
}
