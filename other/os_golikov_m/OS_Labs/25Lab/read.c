#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define READ_ERROR -1
#define STDIN_END 0
#define BUFSIZE 256

void StringToUpper(char *buf) {
	int i;
	for(i = 0; buf[i] != '\0'; i++) {
		buf[i] = toupper(buf[i]);
	}
}

void CleanString(char *str) {
	int i = 0;
	for(; str[i] != 0; i++) {
		str[i] = 0;
	}
}

int main() {
	int error;
	char buf[BUFSIZE + 1] = { 0 };
	while(buf[0] != '.') {
		CleanString(buf);
		error = read(STDIN_FILENO, buf, BUFSIZE);
		if(error == READ_ERROR) {
			perror("Read error");
			return EXIT_FAILURE;
		}
		StringToUpper(buf);
		printf("%s\n", buf);
	}
	fclose(stdin);
	return EXIT_SUCCESS;
}
