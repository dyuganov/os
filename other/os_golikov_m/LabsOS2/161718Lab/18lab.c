#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#define PTHREAD_SUCCESS 0
#define BUFFER_SIZE 80
#define list_t struct list
#define STRING_COUNT 100
#define SORT_NEEDED 1
#define SORT_NOT_NEEDED 0
#define SLEEP_TIME 5
#define STRING_BIGGER 1

list_t {
	list_t* next;
	char* text;
	pthread_mutex_t* mutex;
};

list_t *list;
int sort_needed = SORT_NEEDED;

void initialize(list_t* node) {
	node->next = NULL;
	node->text = NULL;
	node->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(node->mutex, NULL);
}

void free_list(list_t* head) {
	list_t* next = head->next;
	while(next != NULL) {
		pthread_mutex_destroy(head->mutex);
		free(head->text);
		free(head);
		head = next;
		next = next->next;
	}
}

void str_replace_char(char* str, char old, char new) {
	while(*str != '\0') {
		if(*str == old) {
			*str = new;
		}
		str++;
	}
}

int is_not_sorted(list_t* head) {
	if(head->next == NULL) {
		return SORT_NOT_NEEDED;
	}
	head = head->next;
	while(head->next != NULL) {
		if(strcmp(head->text, head->next->text) == STRING_BIGGER) {
			return SORT_NEEDED;
		}
		head = head->next;
	}
	return SORT_NOT_NEEDED;
}

void print_list(list_t* head) {
	list_t* node = head->next;
	printf("Start of list\n");
	for(; node != NULL; node = node->next) {
		pthread_mutex_lock(node->mutex);
		printf("%s\n", node->text);
		pthread_mutex_unlock(node->mutex);
	}
	printf("End of list\n");
}

void sort_list(list_t* head) {
	list_t* current_node;
	list_t* previous_node;
	while(is_not_sorted(head)) {
		current_node = head->next;
		previous_node = head;
		while(current_node->next != NULL) {
			pthread_mutex_lock(previous_node->mutex);
			pthread_mutex_lock(current_node->mutex);
			pthread_mutex_lock(current_node->next->mutex);
			if(strcmp(current_node->text, current_node->next->text) > 0) {
				previous_node->next = current_node->next;
				current_node->next = previous_node->next->next;
				previous_node->next->next = current_node;
				pthread_mutex_unlock(current_node->mutex);
				pthread_mutex_unlock(previous_node->next->mutex);
				pthread_mutex_unlock(previous_node->mutex);
				previous_node = previous_node->next;
				print_list(list);
				sleep(1);
				continue;
			}
			pthread_mutex_unlock(current_node->next->mutex);
			pthread_mutex_unlock(current_node->mutex);
			pthread_mutex_unlock(previous_node->mutex);
			current_node = current_node->next;
			previous_node = previous_node->next;
		}
	}
}

void* thread_control(void* arg) {
	while(sort_needed) {
		sleep(SLEEP_TIME);
		sort_list(list);
	}
	return NULL;
}

int main() {
	int result, i = 0;
	char buffer[BUFFER_SIZE + 1] = { 0 };
	list = (list_t*)malloc(sizeof(list_t));
	initialize(list);
	pthread_t child;
	result = pthread_create(&child, NULL, thread_control, NULL);
	if(result != PTHREAD_SUCCESS) {
		errno = result;
		perror("Error while creating a thread");
		return EXIT_FAILURE;
	}
	/*pthread_t childs[4];
	for(; i < 4; i++) {
		result = pthread_create(&childs[i], NULL, thread_control, NULL);
		if(result != PTHREAD_SUCCESS) {
			errno = result;
			perror("Error while creating a thread");
			return EXIT_FAILURE;
		}
	}*/
	for(i = 0; i < STRING_COUNT; i++) {
		char* str = (char*)calloc(0, BUFFER_SIZE + 1);
		fgets(str, BUFFER_SIZE, stdin);
		if(strcmp(str, "\n") == 0) {
			print_list(list);
			continue;
		}
		str_replace_char(str, '\n', '\0');
		list_t* node = (list_t*)malloc(sizeof(list_t));
		initialize(node);
		node->text = str;
		node->next = list->next;
		pthread_mutex_lock(list->mutex);
		list->next = node;
		pthread_mutex_unlock(list->mutex);
	}
	sort_needed = SORT_NOT_NEEDED;
	pthread_join(child, NULL);
	/*for(i = 0; i < 4; i++) {
		pthread_join(childs[i], NULL);
	}*/
	free_list(list);
	pthread_exit(NULL);
}