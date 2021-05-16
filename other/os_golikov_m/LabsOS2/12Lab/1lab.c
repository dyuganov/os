#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define PTHREAD_CREATE_SUCCESS 0
#define LINES_COUNT 10

char* stringForParentThread = "Parent string\n";
char* stringForChildThread = "Child string\n";

void print_lines(char* line, int count) {
	int i = 0;
	for(; i < count; i++) {
		printf("%s", line);
	}
}

void* thread_control(void* arg) {
	print_lines(stringForChildThread, LINES_COUNT);
	return NULL;
}

int main() {
	int result;
	pthread_t child;
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_CREATE_SUCCESS) {
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	print_lines(stringForParentThread, LINES_COUNT);
	pthread_exit(NULL);
}
