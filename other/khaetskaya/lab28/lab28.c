#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>

int main() {
	srand(time(NULL));
	FILE * fd[2];
	p2open("sort", fd); 

	int currentNum;

	printf("Generated array:\n");
	for(int i = 0; i < 10; i ++){
		for(int j = 0; j < 10; j++){
			currentNum = rand() % 100;
			fprintf(fd[0], "%d\n", currentNum);
			printf("%3d ", currentNum); 
		}
		printf("\n");
	}
	pclose(fd[0]);

	printf("\n");

	printf("Sorted array:\n");
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			currentNum = rand() % 100;
			fscanf(fd[1], "%d", &currentNum);
			printf("%3d ", currentNum);
		}
		printf("\n");
	}

	pclose(fd[1]);
	return 0;
}

