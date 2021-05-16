#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_CANCEL_SUCCESS 0

char* stringForChildThread = "Child string\n";

void print_lines(char* line) {
	while(1) {
		printf("%s", line);
		usleep(300000);
	}
}

void on_cancel(void* arg) {
	printf("This thread is canceled by parent!\n");
}

void* thread_control(void* arg) {
	pthread_cleanup_push(on_cancel, NULL);
	print_lines(stringForChildThread);
	pthread_cleanup_pop(1);
	return NULL;
}

int main() {
	int result;
	pthread_t child;
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_CREATE_SUCCESS) {
		errno = result;
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	sleep(2);
	result = pthread_cancel(child);
	if(result != PTHREAD_CANCEL_SUCCESS) {
		errno = result;
		perror("Error canceling child thread");
		return EXIT_FAILURE;
	}
	pthread_exit(NULL);
}
