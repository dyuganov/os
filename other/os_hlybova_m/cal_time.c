#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PUTENV_CORRECT 0
#define TIME_ERROR -1

int main(int argc, char **argv){
	// /usr/share/zoneinfo/
	int putenvReturn = putenv("TZ=:America/Los_Angeles"); 
	if (putenvReturn != PUTENV_CORRECT){
		perror("Set TZ error");
		return EXIT_FAILURE;
	}
	
	time_t currentCalTime;
	time_t returnTime = time(&currentCalTime);
	if (returnTime == (time_t)TIME_ERROR){
		perror("Time error");
		return EXIT_FAILURE;
	}
	
	char *strTime = ctime(&currentCalTime);
	if (strTime == NULL){
		perror("Cannot convert time to string");
		return EXIT_FAILURE;
	}
	
	 printf("Current local time and date in California: %s", strTime);

    return EXIT_SUCCESS;
}