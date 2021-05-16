#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <sys/mman.h>

#define OPEN_ERROR -1
#define LSEEK_ERROR -1
#define CLOSE_ERROR -1
#define LINES_MAX_AMOUNT 100
#define READ_ERROR -1
#define WRONG_INPUT -1
#define CLOSE_ERROR -1
#define TIME_WAIT 5
#define MUNMAP_ERROR -1

typedef struct {
    off_t offsetBytes;
    int length;
} FileLine;

void printAllLines(char *mappingFile, size_t fileSize){
	int i;
	for (i = 0; i < fileSize; i++){
		int returnPrintf = printf("%c", mappingFile[i]);
		if (returnPrintf < 0){
			perror("printf error");
		}
	}
}

void printSignal(int signumb){
	printf("It's too late for response. File:\n");
}

int parseFile(char *mappingFile, size_t fileSize, FileLine *lines, int *linesAmount){
	size_t i;
	for (i = 0; i < fileSize; i++)
	{		
		if (mappingFile[i] == '\n'){
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
	int returnPrintf = printf("Enter the string number, you have 5 seconds: (0 for exit)\n");
	if (returnPrintf < 0){
		perror("Printf error");
	}
}

int findLineAndPrint(char *mappingFile, size_t fileSize, FileLine *lines, int needLine){
	off_t beginPosition = lines[needLine - 1].offsetBytes;
	off_t endPosition = beginPosition + lines[needLine - 1].length;
	off_t index;
	for(index = beginPosition; index < endPosition; index++) {
            printf("%c", mappingFile[index]);
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

int printLines(char *mappingFile, size_t fileSize, FileLine *lines, int linesAmount){
	int needLine;
	char line[BUFSIZ];
	
	if(signal(SIGALRM, printSignal) == SIG_ERR) {
        perror("Signal error");
        return EXIT_FAILURE;
    }
	printIntroduction();
	alarm(TIME_WAIT);
	char *returnFgets = fgets(line, BUFSIZ, stdin);
	while(returnFgets != NULL){
		alarm(0);
		int needLine = getNumb(line);
		if (needLine == 0){
			return EXIT_SUCCESS;
		}
		if (needLine > 0 && needLine <= linesAmount){
			findLineAndPrint(mappingFile, fileSize, lines, needLine);
		}
		if (needLine == WRONG_INPUT){
			printf("Wrong input, try again\n");
		}
		if  ((needLine > linesAmount || needLine < 0) && (needLine != WRONG_INPUT)){
			printf("Wrong number\n");
		}
		printIntroduction();
		alarm(TIME_WAIT);
		returnFgets = fgets(line, BUFSIZ, stdin);
	}
	if (errno == EINTR){ //if fgets was interrupted by SIGALRM
		printAllLines(mappingFile, fileSize);
		return EXIT_SUCCESS;
	}
	perror("fgets error");
	return EXIT_FAILURE;
}

void printLinesAmount(int linesAmount){
	int returnPrintf = printf("File has %d lines.\n", linesAmount);
	if (returnPrintf < 0){
		perror("Printf error");
	}
}

char *mapping(int fileDescriptor, size_t *fileSize){ 
	off_t lseekResult = lseek(fileDescriptor, 0, SEEK_END);
	if (lseekResult == (off_t)LSEEK_ERROR){
		perror("lseek error");
		return NULL;
	}
	*fileSize = (size_t)lseekResult;
	char *mappingFile = (char *)mmap(NULL, *fileSize, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);
	if ((void *)mappingFile == MAP_FAILED){
		perror("mmap error");
		return NULL;
	}
	
	return mappingFile;
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
	
	size_t fileSize;
	char *mappingFile = mapping(descriptorOpenFile, &fileSize);
	
	if (mappingFile == NULL){
		closeFile(descriptorOpenFile);
		return EXIT_FAILURE;
	}
	
	FileLine lines[LINES_MAX_AMOUNT];
	int linesAmount = 0;

	int resultParse = parseFile(mappingFile, fileSize, lines, &linesAmount);
	
	if (resultParse != EXIT_SUCCESS){
		closeFile(descriptorOpenFile);
		return EXIT_FAILURE;
	}

	if (linesAmount == 0){
		fprintf(stderr, "File is empty\n");
		return EXIT_FAILURE;
	}
	
	printLinesAmount(linesAmount);
	
	int returnPrintLines = printLines(mappingFile, fileSize, lines, linesAmount);
	if (returnPrintLines == EXIT_FAILURE){
		closeFile(descriptorOpenFile);
		return EXIT_FAILURE;
	}
	closeFile(descriptorOpenFile);
	
	int munmapResult = munmap(mappingFile, fileSize);
	if (munmapResult == MUNMAP_ERROR){
		perror("munmap error");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
