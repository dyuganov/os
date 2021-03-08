#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void openAndCloseFile(const char* fileName){
	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL){
		perror("Can't open file.");
		return;
	}

	const int resultFclose = fclose(file);
  const int FCLOSE_SUCCESS = 0;
	if (resultFclose != FCLOSE_SUCCESS){
		perror("Fclose error.");
		return;
	}
}

int main(int argc, char **argv){
	printf("Real user ID %ld \n", getuid());
	printf("Effective user ID %ld \n", geteuid());

  const int ARGC_CORRECT_VAL = 2;
	if (argc != ARGC_CORRECT_VAL) {
		perror("Need file.");
		return EXIT_FAILURE;
	}

	const short fileNameArgcIdx = 1;
	openAndCloseFile(argv[fileNameArgcIdx]);

	uid_t uid = getuid();
	setuid(uid);

	printf("After using setuid \n");
	printf("Real user ID %ld \n", getuid());
	printf("Effective user ID %ld \n", geteuid());

	openAndCloseFile(argv[fileNameArgcIdx]);

	return EXIT_SUCCESS;
}
