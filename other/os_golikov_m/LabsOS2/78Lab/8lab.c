#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <limits.h>
#include <signal.h>

#define ThreadData struct thread_data
#define MAX_ITERATION_COUNT ULLONG_MAX
#define ITERATION_AFTER_SIGNAL 100000
#define PTHREAD_CREATE_SUCCESS 0
#define STOP_NEEDED 1
#define STOP_NOT_NEEDED 0
#define ITERATIONS_SET 1
#define ITERATIONS_NOT_SET 0

#define STRTOL_ERROR(a) (a == LONG_MIN || a == LONG_MAX)

ThreadData {
	int thread_count;
	int index;
	unsigned long long iteration_count;
	double result;
};

int is_stop_needed = STOP_NOT_NEEDED;
unsigned long long iterations_needed;
pthread_mutex_t iterations_needed_mutex;
pthread_barrier_t iterations_barrier;

void* thread_control(void* arg) {
	ThreadData* data = (ThreadData*)arg;
	unsigned long long i = data->index;
	int is_iteration_set = ITERATIONS_NOT_SET;
	for(; i < iterations_needed; i += data->thread_count) {
		data->iteration_count = i;
		data->result += (i % 2 == 0 ? 1 : -1) / (2*(double)i + 1);
		if(!is_iteration_set && is_stop_needed) {
			is_iteration_set = ITERATIONS_SET;
			pthread_mutex_lock(&iterations_needed_mutex);
			if(iterations_needed == MAX_ITERATION_COUNT / data->thread_count || iterations_needed < data->iteration_count + ITERATION_AFTER_SIGNAL) {
				iterations_needed = data->iteration_count + ITERATION_AFTER_SIGNAL;
			} 
			pthread_mutex_unlock(&iterations_needed_mutex);
			pthread_barrier_wait(&iterations_barrier);
		}
	}
	return NULL;
}

void sigint_handler(int signo) {
	is_stop_needed = STOP_NEEDED;
}

int main(int argc, char** argv) {
	int result, i = 0;
	double pi = 0;
	sigset(SIGINT, sigint_handler);
	int thread_count = strtol(argv[1], (char**)NULL, 10);
	if(STRTOL_ERROR(thread_count)) {
		perror("Wrong thread count");
		return EXIT_FAILURE;
	}
	iterations_needed = MAX_ITERATION_COUNT / thread_count;
	pthread_mutex_init(&iterations_needed_mutex, NULL);
	pthread_barrier_init(&iterations_barrier, NULL, thread_count);
	ThreadData* thread_datas = (ThreadData*)malloc(sizeof(ThreadData) * thread_count);
	pthread_t child[thread_count];
	for(; i < thread_count; i++) {
		thread_datas[i].thread_count = thread_count;
		thread_datas[i].index = i;
		thread_datas[i].result = 0;
		thread_datas[i].iteration_count = 0;
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
		printf("\n%ld", thread_datas[i].iteration_count);
	}
	pi *= 4.0;
	printf("\n%.10f\n", pi);
	free(thread_datas);
	pthread_mutex_destroy(&iterations_needed_mutex);
	pthread_barrier_destroy(&iterations_barrier);
	pthread_exit(NULL);
}
