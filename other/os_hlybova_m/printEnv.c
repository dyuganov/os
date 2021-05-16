#include <stdio.h>
extern char **environ;

int main(int argc, char **argv){
	int i;
	char **ptrEnv = environ;
	printf("My input parameters are:\n");
	for (i = 0; i < argc; i++){
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	printf("\nMy enviroment variables are:\n");
	for (; *ptrEnv != NULL; ptrEnv++){
		printf ("%s\n", *ptrEnv);
	}
}


