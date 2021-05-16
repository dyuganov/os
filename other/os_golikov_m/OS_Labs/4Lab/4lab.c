#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 256
#define MALLOC_ERR NULL
#define FILE_ERR NULL

struct list {
     struct list *next;
     char *data;
};

//Очистка памяти одного элемента списка

void FreeOne(struct list *node) {
    free(node->data);
    free(node);
}

//Очистка всего списка

void FreeList(struct list *head) {
    struct list *tmp, *cur;
	
    for(tmp = head; tmp->next != NULL; tmp = cur) {
		cur = tmp->next;
		FreeOne(tmp);
    }
    FreeOne(tmp);
}

//Создание элемента списка - выделение памяти, инициализация полей

int CreateNode(struct list **node, char *string) {
	*node = (struct list*)malloc(sizeof(struct list));
    if(*node == MALLOC_ERR) {
		perror("Memory error in CreateNode");
		return EXIT_FAILURE;
    }
    (*node)->next = NULL;
	(*node)->data = string;
	return EXIT_SUCCESS;
}

//Добавление элемента в список после последнего элемента last

int AddNode(struct list **head, char *string) {
	int error;
	struct list *temp, *last;
	error = CreateNode(&temp, string);
	if(error == EXIT_FAILURE) return EXIT_FAILURE;
	if(*head == NULL) {
		*head = temp;
		return EXIT_SUCCESS;
	}
	for(last = *head; last->next != NULL; last = last->next);
	last->next = temp;
	return EXIT_SUCCESS;
}

//Считывание строки из стандартного потока ввода stdin

int ReadString(char **string) {
	char buf[STR_MAX+1] = { 0 };
	char *error, *line = NULL, *tmp;
	int len = 1, i;
	
	for(i = 1; buf[len-1] != '\n'; i++) {
		error = fgets(buf, STR_MAX, stdin);
		if(error == FILE_ERR) {
			perror("fgets error in ReadString");
			free(line);
        	return EXIT_FAILURE;
		}
		len = strlen(buf);
		tmp = (char*)realloc(line, STR_MAX*i + 1);
		if(tmp == MALLOC_ERR) {
	    	perror("Memory error in ReadString");
			free(line);
	    	return EXIT_FAILURE;
		}
		line = tmp;
		strcat(line, buf);
	}
	*string = tmp;
    return EXIT_SUCCESS;
}

//Заполнение всего списка 

int FillList(struct list **listHead) {
    int error;
	char *string;
	
	do {
		error = ReadString(&string);
		if(error == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		error = AddNode(listHead, string);
		if(error == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	} while(string[0] != '.');
    return EXIT_SUCCESS;
}

//Вывод всего списка в стандартный поток вывода

void PrintList(struct list *head) {
    struct list *cur;
	
    for(cur = head; cur->next != NULL; cur = cur->next) {
		printf("%s", cur->data);
	}
}

int main(int argc, char** argv) {
    struct list *head = NULL;
    int error;
	
    error = FillList(&head);
    if(error == EXIT_FAILURE) return EXIT_FAILURE;
    PrintList(head);
    FreeList(head);
    return EXIT_SUCCESS;
}
