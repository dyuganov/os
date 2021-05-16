#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define PTHREAD_SUCCESS 0
#define LINES_COUNT 10
#define SECOND_TURN 1
#define START_TURN 0

pthread_mutex_t mutex;
pthread_cond_t cond;

char* stringForParentThread = "Parent string\n";
char* stringForChildThread = "Child string\n";

int init_pthread() {
	int result;
	result = pthread_mutex_init(&mutex, NULL);
	if(result != PTHREAD_SUCCESS) return result;
	result = pthread_cond_init(&cond, NULL);
	if(result != PTHREAD_SUCCESS) return result;
	return PTHREAD_SUCCESS;
}

int destroy_pthread() {
	int result;
	result = pthread_mutex_destroy(&mutex);
	if(result != PTHREAD_SUCCESS) return result;
	result = pthread_cond_destroy(&cond);
	if(result != PTHREAD_SUCCESS) return result;
	return PTHREAD_SUCCESS;
}

int print_lines(char* line, int count, int turn) {
	int i = 0, result;
	result = pthread_mutex_lock(&mutex);
	if(result != PTHREAD_SUCCESS) return result;
	result = pthread_cond_signal(&cond);
	if(result != PTHREAD_SUCCESS) return result;
	for(; i < count; i++) {
		result = pthread_cond_wait(&cond, &mutex);
		if(result != PTHREAD_SUCCESS) return result;
		printf("%s", line);
		result = pthread_cond_signal(&cond);
		if(result != PTHREAD_SUCCESS) return result;
	}
	result = pthread_mutex_unlock(&mutex);
	if(result != PTHREAD_SUCCESS) return result;
	return PTHREAD_SUCCESS;
}

void* thread_control(void* arg) {
	int result = print_lines(stringForChildThread, LINES_COUNT, SECOND_TURN);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error in child thread");
		exit(EXIT_FAILURE);
	}
	return NULL;
}

int main() {
	int result;
	pthread_t child;
	init_pthread();
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	result = print_lines(stringForParentThread, LINES_COUNT, START_TURN);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error in parent thread");
		return EXIT_FAILURE;
	}
	destroy_pthread();
	pthread_exit(NULL);
}
