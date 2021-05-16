#include <stdlib.h>
#include <stdio.h>

#define IS_PRINTF_ERROR(a) (a < 0) ? 1 : 0
#define FFLUSH_ERROR EOF
#define BUFSIZE 256

int main() {
	int error;
	char buf[BUFSIZE+1] = { 0 };
	while(buf[0] != '.') {
		scanf("%s", buf);
		error = printf("%s", buf);
		if(IS_PRINTF_ERROR(error)) {
			perror("Printf error");
			return EXIT_FAILURE;
		}	
		error = fflush(stdout);
		if(error == FFLUSH_ERROR) {
			perror("Fflush error");
			return EXIT_FAILURE;
		}
	}
	fclose(stdout);
	return EXIT_SUCCESS;
}
