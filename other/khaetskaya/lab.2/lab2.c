#include <sys/types.h>
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
//extern char *tzname[]; 
int main() {     
	time_t now;     
	struct tm *sp;

	// In Pacific Standard Time (PST) and Pacific Daylight Time (PDT),
	// the normal offset from UTC is 8 hours; since this is west of the prime meridian,
	// the sign is positive. Summer time begins on March's second Sunday at 2:00am, 
	// and ends on November's first Sunday at 2:00am
	putenv ("TZ=Hongkong");

	(void) time( &now );
        //printf("%s", ctime( &now ) );     
	sp = localtime(&now);
     
	printf("%d/%d/%02d %d:%02d %s\n", 
					sp->tm_mon + 1, 
					sp->tm_mday, 
					sp->tm_year, 
					sp->tm_hour, 
					sp->tm_min, 
					tzname[sp->tm_isdst]);

   return 0;
}
