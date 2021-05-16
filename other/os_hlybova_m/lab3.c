#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define FCLOSE_SUCCESS 0

void openAndCloseFile(const char* fileName){
	FILe *file;
	file = fopen(fileName, "r");
	if (file == NULL){
		perror("Can't open the file.");
		return;
	}

	int resultFclose = fclose(file);
	if (resultFclose != FCLOSE_SUCCESS){
		perror("fclose error");
	}
}

int main(int argc, char **argv){
	printf("real user ID %ld \n", getuid());
	printf("effective user ID %ld \n", geteuid());

	if (argc != 2) {
		fprintf(stderr, "Need file\n");
		return EXIT_FAILURE;
	}

	openAndCloseFile(argv[1]);

	uid_t uid = getuid();
	setuid(uid);

	printf("after using setuid \n");
	printf("real user ID %ld \n",getuid());
	printf("effective user ID %ld \n",geteuid());

	openAndCloseFile(argv[1]);

	return EXIT_SUCCESS;
}
