#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define PTHREAD_SUCCESS 0
#define LINES_COUNT 10
#define MUTEX_COUNT 3
#define PARENT_INDEX 0
#define CHILD_INDEX 1

pthread_mutex_t mutex[MUTEX_COUNT];

char* stringForParentThread = "Parent string\n";
char* stringForChildThread = "Child string\n";

int init_mutexes() {
	int i = 0, result;
	for(; i < MUTEX_COUNT; i++) {
		result = pthread_mutex_init(&mutex[i], NULL);
		if(result != PTHREAD_SUCCESS) return result;
	}
	return PTHREAD_SUCCESS;
}

int destroy_mutexes() {
	int i = 0, result;
	for(; i < MUTEX_COUNT; i++) {
		result = pthread_mutex_destroy(&mutex[i]);
		if(result != PTHREAD_SUCCESS) return result;
	}
	return PTHREAD_SUCCESS;
}

int mutex_lock(int index) {
	return pthread_mutex_lock(&mutex[index]);
}

int mutex_unlock(int index) {
	return pthread_mutex_unlock(&mutex[index]);
}

int print_lines(char* line, int count, int thread_index) {
	int i = 0, result;
	for(; i < count; i++) {
		result = mutex_lock((2 * i + thread_index) % 3); //CHILD_INDEX = 1
		//result = mutex_lock((i + 1 + thread_index) % 3); //CHILD_INDEX = 2
		if(result != PTHREAD_SUCCESS) return result;
		printf("%s", line);
		result = mutex_unlock((2 * i + 1 + thread_index) % 3);
		//result = mutex_unlock((i + thread_index) % 3);
		if(result != PTHREAD_SUCCESS) return result;
	}
	return PTHREAD_SUCCESS;
}

void* thread_control(void* arg) {
	mutex_lock(2);
	int result = print_lines(stringForChildThread, LINES_COUNT, CHILD_INDEX);
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
	init_mutexes();
	mutex_lock(1);
	//mutex_lock(0); 
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	sleep(1);
	result = print_lines(stringForParentThread, LINES_COUNT, PARENT_INDEX);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error in parent thread");
		return EXIT_FAILURE;
	}
	destroy_mutexes();
	pthread_exit(NULL);
}
