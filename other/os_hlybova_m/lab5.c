#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

#define OPEN_ERROR -1
#define LSEEK_ERROR -1
#define CLOSE_ERROR -1
#define LINES_MAX_AMOUNT 100
#define READ_ERROR -1
#define WRONG_INPUT -1
#define CLOSE_ERROR -1

typedef struct {
    off_t offsetBytes;
    int length;
} FileLine;

int parseFile(int fileDescriptor, FileLine *lines, int *linesAmount){
	char character = 0;
	ssize_t returnRead = read(fileDescriptor, &character, sizeof(char));
	while (returnRead > 0)
	{		
		if (character == '\n'){
			++*linesAmount;
			lines[*linesAmount].length = 0;
			lines[*linesAmount].offsetBytes = lines[*linesAmount - 1].offsetBytes + lines[*linesAmount - 1].length + 1;
		} else{
			lines[*linesAmount].length++;
		}
		if (*linesAmount > LINES_MAX_AMOUNT){
			fprintf(stderr, "File is too big\n");
			return EXIT_FAILURE;
		}
		returnRead = read(fileDescriptor, &character, sizeof(char));
	}

	if (returnRead == READ_ERROR){
		perror("Can't read the file");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

void closeFile(int fileDescriptor){
	int returnClose = close(fileDescriptor);
	if (returnClose == CLOSE_ERROR){
		perror("Can't close the file");
	}
}

void printIntroduction(){
	int returnPrintf = printf("Enter the string number: (0 for exit)\n");
	if (returnPrintf < 0){
		perror("Printf error");
	}
}

int findLineAndPrint(int fileDescriptor, FileLine *lines, int needLine){
	char lineCharacter;
	int i;
	off_t resultLseek = lseek(fileDescriptor, lines[needLine - 1].offsetBytes, SEEK_SET);
	if (resultLseek == (off_t)LSEEK_ERROR){
		perror("Lseek error");
		return EXIT_FAILURE;
	}
		
	for (i = 0; i < lines[needLine - 1].length; i++){
		ssize_t returnRead = read(fileDescriptor, &lineCharacter, sizeof(char));
		if(returnRead == READ_ERROR) {
            perror("Can't read file");
            return EXIT_FAILURE;
        }
        printf("%c", lineCharacter);
		}
	printf("\n");
	return EXIT_SUCCESS;
}

int getNumb(char *line){
	char *end;
	errno = 0;
	long returnStrtol = strtol(line, &end, 10);
	// nothing parsed from the string, handle errors or exit
	if (end == line){
		return WRONG_INPUT;
	}
	if ((returnStrtol == LONG_MAX || returnStrtol == LONG_MIN) && errno == ERANGE)
	{
   	 // out of range, handle or exit
		return WRONG_INPUT;
	}
	char charAfterNumb; 
	sscanf(end, "%c", &charAfterNumb);
	if (charAfterNumb != '\n')
		return WRONG_INPUT;
	return (int)returnStrtol;
}

int printLines(int fileDescriptor, FileLine *lines, int linesAmount){
	int needLine;
	printIntroduction();
	char line[BUFSIZ];
	char *returnFgets = fgets(line, BUFSIZ, stdin);
	while (returnFgets != NULL){
		int needLine = getNumb(line);
		if (needLine == 0){
			return EXIT_SUCCESS;
		}
		if (needLine > 0 && needLine < linesAmount){
			findLineAndPrint(fileDescriptor, lines, needLine);
		}
		if (needLine == WRONG_INPUT){
			printf("Wrong input, try again\n");
		}
		if  ((needLine > linesAmount || needLine < 0) && (needLine != WRONG_INPUT)){
			printf("Wrong number\n");
		}
		printIntroduction();
		returnFgets = fgets(line, BUFSIZ, stdin);
	}
	perror("Fgets error");
	return EXIT_FAILURE;
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Need filename\n");
		return EXIT_FAILURE;
	}
	
	int descriptorOpenFile = open(argv[1], O_RDONLY);
	if (descriptorOpenFile == OPEN_ERROR){
		perror("Can't open the file");
		return EXIT_FAILURE;
	}	

	FileLine lines[LINES_MAX_AMOUNT];
	int linesAmount = 0;

	int resultParse = parseFile(descriptorOpenFile, lines, &linesAmount);
	
	if (resultParse != EXIT_SUCCESS){
		closeFile(descriptorOpenFile);
		return EXIT_FAILURE;
	}

	if (linesAmount == 0){
		fprintf(stderr, "File is empty\n");
		return EXIT_FAILURE;
	}
	
	int returnPrintLines = printLines(descriptorOpenFile, lines, linesAmount);
	if (returnPrintLines == EXIT_FAILURE){
		closeFile(descriptorOpenFile);
		return EXIT_FAILURE;
	}
	closeFile(descriptorOpenFile);
	return EXIT_SUCCESS;
}
