#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define PTHREAD_CREATE_SUCCESS 0
#define THREAD_COUNT 4
#define STRING_COUNT 3

char* stringForThread[THREAD_COUNT][STRING_COUNT] = { 
{"a1\n", "a2\n", "a3\n"}, 
{"b1\n", "b2\n", "b3\n"}, 
{"c1\n", "c2\n", "c3\n"}, 
{"d1\n", "d2\n", "d3\n"} };

void* thread_control(void* arg) {
	int i = 0;
	for(; i < STRING_COUNT; i++) {
		printf("%s", ((char**)arg)[i]);
	}
	pthread_exit(NULL);
}

int main() {
	int result, i = 0;
	pthread_t children[THREAD_COUNT];
	for(; i < THREAD_COUNT; i++) {
		result = pthread_create(&children[i], NULL, thread_control, stringForThread[i]);
		if(result != PTHREAD_CREATE_SUCCESS) {
			fprintf(stderr, "Error while creating a thread");
			return EXIT_FAILURE;
		}
	}
	pthread_exit(EXIT_SUCCESS);
}
