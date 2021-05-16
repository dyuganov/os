#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>

#define NOT_TIMEOUT 1
#define STD_OUT 1
#define BUFSIZE 257

int main(int argc, char *argv[])
{
     struct timeval currentTime;
     static char *fileOffsets[501];
     char *p, buf[10];
     int fd1, fd2, count, i = 1, j = 1, lineNumber, lineLengths[500];
     static char err_msg[32] = "Input file - ";
     off_t fileSize;

     if ((fd1 = open("/dev/tty", O_RDONLY | O_NDELAY)) == -1) {
         perror("/dev/tty");
         exit(1);
     }

     if ((fd2 = open(argv[1], O_RDONLY)) == -1) {
         perror(strcat(err_msg, argv[1]));
         exit(1);
         }

     fileSize = lseek(fd2, 0, SEEK_END);
     p = mmap(0, fileSize, PROT_READ, MAP_SHARED, fd2, 0);

     fileOffsets[1] = p;
     for(count = 0; count < fileSize; count++)
         if( *(p + count) == '\n' ) {
             lineLengths[i++] = j;
             fileOffsets[i] = count + p + 1;
             j = 1;
         }
         else
             j++;
     fileOffsets[i] = 0;
     printf("five seconds to write a line number\n");
     int timeCountedFrom = 0;
     gettimeofday(&currentTime, NULL);
     timeCountedFrom = currentTime.tv_sec;
     while (NOT_TIMEOUT) {
                gettimeofday(&currentTime, NULL);
                if((currentTime.tv_sec - timeCountedFrom) >= 5){
                        write(STD_OUT, p, fileSize);
			break;
                }
                if ((i = read (fd1, buf, BUFSIZE)) > 0) {
                        buf[i] = '\0';
                        lineNumber = atoi(buf);
                        if(lineNumber == 0)
                                exit(0);
                        if(lineNumber < 0 || lineNumber > 100){
                                fprintf(stderr, "wrong line number\n");
				timeCountedFrom = currentTime.tv_sec;
                                continue;
                        }
                        if(fileOffsets[lineNumber] != 0){
                                write(STD_OUT, fileOffsets[lineNumber], lineLengths[lineNumber]);
                        }
                        else {
                                fprintf(stderr, "wrong line number\n");
                        }
                        gettimeofday(&currentTime, NULL);
                        timeCountedFrom = currentTime.tv_sec;
                }
        }
     return 0;
}
/*
     while (NOT_TIMEOUT) {
         else {
             buf[i] = '\0';
             lineNumber = atoi(buf);
             if(lineNumber == 0)
                 exit(0);
	     if(lineNumber < 0 || lineNumber > 100){
		 fprintf(stderr, "wrong line number\n");
	         continue;
	     }
             if(fileOffsets[lineNumber] != 0)
                 write(STD_OUT, fileOffsets[lineNumber], lineLengths[lineNumber]);
             else
                 fprintf(stderr, "wrong line number\n");
         }
     }
}
*/
