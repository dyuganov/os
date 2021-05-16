#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <unistd.h>

#define PIPE_ERROR -1
#define FORK_ERROR -1
#define CHILD_PROCESS 0
#define WAIT_ERROR -1
#define DUP_ERROR -1

/*
В данной программе используются следующие системные вызовы:

int pipe(int fildes[2]);
	1. fildes - массив файловых дескрипторов, куда нужно поместить открытые концы канала
	2. Возвращает 0 при успехе, иначе -1 и устанавливает errno {
		- EMFILE - достигнуто максимальное количество открытых процессом файлов
		- ENFILE - достигнуто максимальное количество открытых файлов в системе
		- EFAULT - недействительный аргумент
	}
Системный вызов pipe создает канал. Оба конца канала помещаются в переданный массив. Также оба конца открыты для чтения и записи.

int close(int fildes);
	1. fildes - файловый дескриптор, который нужно закрыть
	2. Возвращает 0 при успехе, иначе -1 и устанавливает errno {
		- EBADF - передан недействительный файловый дескриптор
		- EIO - ошибка ввода/вывода при чтении/записи в файловой системе
	}
Системный вызов close закрывает переданный ему файловый дескриптор. После закрытия этот файловый дескриптор может быть переиспользован.

int dup(int fildes);
	1. fildes - файловый дескриптор, который нужно скопировать
	2. Возвращает новый файловый дескриптор при успехе, иначе -1 и устанавливаеn errno {
		- EBADF - передан недействительный файловый дескриптор
		- EMFILE - достигнуто максимальное количество открытых процессом файлов
	}
Системный вызов dup копирует переданный дескриптор, т.е. новый файловый дескриптор будет ссылаться на ту же системную файловую структуру, что и переданный. dup создает дескриптор с наименьшим возможным значением. 

Системные вызовы fork и семейство системных вызовов exec рассматривались ранее. 
*/

/*
Функция main

В данной функции сначала создается канал, далее создаются два подпроцесса, в первом из них закрывается стандартный поток вывода и с помощью dup на бывший дескриптор stdout назначается один из концов созданного канала, во втором закрывается стандартный поток ввода и на его место назначается другой конец канала.
*/

int main() {
	int fildes[2], error, forkResult;
	char writeProg[] = "./write", readProg[] = "./read";
	error = pipe(fildes);
	if(error == PIPE_ERROR) {
		perror("Pipe error");
		return EXIT_FAILURE;
	}
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		close(STDOUT_FILENO);
		error = dup(fildes[1]);
		if(error == DUP_ERROR) {
			perror("dup error");
			return EXIT_FAILURE;
		}
		execlp(writeProg, writeProg, NULL);
	}
	forkResult = fork();
	if(forkResult == FORK_ERROR) {
		perror("Creating child process error");
		return EXIT_FAILURE;
	}
	if(forkResult == CHILD_PROCESS) {
		close(STDIN_FILENO);
		error = dup(fildes[0]);
		if(error == DUP_ERROR) {
			perror("dup error");
			return EXIT_FAILURE;
		}
		execlp(readProg, readProg, NULL);
	}
	error = wait(NULL);
	if(error == WAIT_ERROR) {
		perror("Wait error");
		return EXIT_FAILURE;
	}
	error = wait(NULL);
	if(error == WAIT_ERROR) {
		perror("Wait error");
		return EXIT_FAILURE;
	}
	printf("Child processes ended\n");
	return EXIT_SUCCESS;
}
