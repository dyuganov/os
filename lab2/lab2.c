#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern char *tzname[];

int main() {
    if(putenv("TZ=America/Los_Angeles")){
        printf("putenv error\n");
        exit(0);
    }

    time_t now; // перемнная time_t хранит разницу в секундах между
    struct tm *sp; //
    (void) time( &now ); // time сохраняет время в секундах от 1 января 1970
    printf("%s", ctime( &now ) ); // переводит время в локальное
    sp = localtime(&now); // корректирует время в соответствии с основным часовым поясом

    printf("%d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year, sp->tm_hour, // tm_year - год от 1900
        sp->tm_min, tzname[sp->tm_isdst]); //tm_isdst - флаг, которй больше 0, если есть сезонное изменение времени,
                                           // равен 0 , если его нет и меньше 0, если нет информации
                                           // по этому индексу из tzname получаем разницу в часах от Гринвича
    exit(0);
}
