#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>

#define ThreadData struct thread_data

#define PTHREAD_CREATE_SUCCESS 0
#define ITERATION_COUNT 1000000

#define STRTOL_ERROR(a) (a == LONG_MIN || a == LONG_MAX)

ThreadData {
	int thread_count;
	int index;
	double result;
};

void* thread_control(void* arg) {
	ThreadData* data = (ThreadData*)arg;
	int i = data->index;
	for(; i < ITERATION_COUNT; i += data->thread_count) {
		data->result += (i % 2 == 0 ? 1 : -1) / (2*(double)i + 1);
	}
	return NULL;
}

int main(int argc, char** argv) {
	int result, i = 0;
	double pi = 0;
	int thread_count = strtol(argv[1], (char**)NULL, 10);
	if(STRTOL_ERROR(thread_count)) {
		perror("Wrong thread count");
		return EXIT_FAILURE;
	}
	ThreadData* thread_datas = (ThreadData*)malloc(sizeof(ThreadData) * thread_count);
	pthread_t child[thread_count];
	for(; i < thread_count; i++) {
		thread_datas[i].thread_count = thread_count;
		thread_datas[i].index = i;
		thread_datas[i].result = 0;
		result = pthread_create(&child[i], NULL, thread_control, &thread_datas[i]);
		if(result != PTHREAD_CREATE_SUCCESS) {
			errno = result;
			perror("Error while creating a thread");
			return EXIT_FAILURE;
		}
	}
	for(i = 0; i < thread_count; i++) {
		pthread_join(child[i], NULL);
		pi += thread_datas[i].result;
	}
	pi *= 4.0;
	printf("%lf\n", pi);
	free(thread_datas);
	pthread_exit(NULL);
}
