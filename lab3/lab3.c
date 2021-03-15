#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void openAndCloseFile(const char* fileName){
	FILE* file = NULL;
	file = fopen(fileName, "r");
	if (file == NULL){
		perror("Can't open file");
		return;
	}
	printf("File opened successfully \n");

	const int resultFclose = fclose(file);
  const int FCLOSE_SUCCESS = 0;
	if (resultFclose != FCLOSE_SUCCESS){
		perror("Fclose error");
		return;
	}
}

int main(int argc, char **argv){
	printf("Real user ID %ld \n", getuid());
	printf("Effective user ID %ld \n", geteuid());

	const int ARGC_CORRECT = 2;
	if (argc != ARGC_CORRECT) {
		perror("Need file");
		return EXIT_FAILURE;
	}

	printf("Trying to open the file: \n");
	const short fileNameArgcIdx = 1;
	openAndCloseFile(argv[fileNameArgcIdx]);

	const int SETUID_ERR = -1;
	uid_t uid = getuid();
	int setuidResult = setuid(uid);
	if(setuidResult == SETUID_ERR){
		perror("setuid error. Wrong id");
	}

	printf("After using setuid \n");
	printf("Real user ID %ld \n", getuid());
	printf("Effective user ID %ld \n", geteuid());

	printf("Trying to open the file: \n");
	openAndCloseFile(argv[fileNameArgcIdx]);

	return EXIT_SUCCESS;
}
