#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <strings.h>
#include <stdlib.h>

#define OPEN_ERROR -1
#define NOT_TERMINAL 0
#define CLOSE_ERROR -1
#define TCGETATTR_ERROR -1
#define TCSETATTR_ERROR -1
#define SYMBS_AMOUNT 1
#define WRITE_ERROR -1
#define READ_ERROR -1

void closeFile(int fileDescriptor){
	int returnClose = close(fileDescriptor);
	if (returnClose == CLOSE_ERROR){
		perror("Can't close the file");
	}
}

int myWrite(int fileDesc, void *buffer, size_t nbyte){
	ssize_t writeReturn = write(fileDesc, buffer, nbyte);
	if (writeReturn == WRITE_ERROR){
		perror("write error");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int getInput(int terminalDesc){
	char question[] = "Are you going to dinner? (Answer y or n)\n";
	char answer[SYMBS_AMOUNT];
	char answerForm[] = "You answered: ";
	char newLine[] = "\n";
	
	int writeReturn = myWrite(terminalDesc, question, strlen(question));
	if (writeReturn == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	
	int readReturn = read(terminalDesc, answer, SYMBS_AMOUNT);
	if (readReturn == READ_ERROR){
		perror("read error");
		return EXIT_FAILURE;
	}
	
	writeReturn = myWrite(terminalDesc, answerForm, strlen(answerForm));
	if (writeReturn == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	
	writeReturn = myWrite(terminalDesc, answer, SYMBS_AMOUNT);
	if (writeReturn == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	
	writeReturn = myWrite(terminalDesc, newLine, strlen(newLine));
	if (writeReturn == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int main(){
	int terminalDescriptor = open("/dev/tty", O_RDWR);
	if (terminalDescriptor == OPEN_ERROR){
		perror("open error");
		return EXIT_FAILURE;
	}
	
	int isattyReturn = isatty(terminalDescriptor);
	if (isattyReturn == NOT_TERMINAL){
		perror("terminalDescriptor is not a terminal");
		closeFile(terminalDescriptor);
		return EXIT_FAILURE;
	}
	
	int getInputReturn = getInput(terminalDescriptor);

	if (getInputReturn == EXIT_FAILURE){
		closeFile(terminalDescriptor);
		return EXIT_FAILURE;
	}
	
	closeFile(terminalDescriptor);
	return EXIT_SUCCESS;
}
