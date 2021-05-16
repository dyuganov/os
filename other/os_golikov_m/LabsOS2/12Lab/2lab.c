#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_JOIN_SUCCESS 0
#define LINES_COUNT 10
#define MALLOC_ERROR NULL

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
	char* str = "abc";
	int len = strlen(str)+1;
	char* res = (char*)malloc(len);
	if(res == MALLOC_ERROR) {
		perror("Memory error in child thread");
		pthread_exit(NULL);
	}
	memcpy(res, str, len);
	pthread_exit(res);
	//return str;
}

int main() {
	int result;
	pthread_t child;
	char* childStr;
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_CREATE_SUCCESS) {
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	result = pthread_join(child, (void**)&childStr);
	if(result != PTHREAD_JOIN_SUCCESS) {
		fprintf(stderr, "Error while joining a thread");
		return EXIT_FAILURE;
	}
	print_lines(stringForParentThread, LINES_COUNT);
	printf("%s\n", childStr);
	free(childStr);
	return EXIT_SUCCESS;
}
