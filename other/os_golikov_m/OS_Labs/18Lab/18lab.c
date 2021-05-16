#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

#define NO_ARGS 1
#define DATE_MAX 50
#define STRFTIME_ERROR 0
#define STAT_ERROR -1
#define GETPWUID_ERROR NULL
#define GETGRGID_ERROR NULL
#define LOCALTIME_ERROR NULL

/*
В данной программе используются следующие системные вызовы и библиотечные функции:

int stat(const char *restrict path, struct stat *restrict buf);
	1. path - путь до файла
	2. buf - указатель на структуру stat, в которую нужно записать информацию о файле
	3. Возвращает 0 в случае успеха, иначе -1 и устанавливает errno {
		- EIO - ошибка при чтении из файловой системы
		- EOVERFLOW - размер файла или количество выделенных для файла блоков не могут быть корректно записаны в struct stat
		- EACCES - ошибка доступа к файла
		- EFAULT - аргументы указывают на недоступные адреса памяти
		- ELOOP - при обработке path был найден цикл из символических ссылок
		- ENAMETOOLONG - длина path превышает максимальную допустимую длины имени файла
		- ENOENT - не существует файла с именем path
	}
Системный вызов stat считывает информацию о файле path и записывает в структуру stat по указателю buf. Прав на чтение, запись и выполнение файла path не нужны, но нужны права на чтение всех директорий в пути path.
В struct stat хранится:
	1. mode_t st_mode - права доступа, тип файла
	2. ino_t st_ino - номер инода 
	3. uid_t st_uid - uid владельца
	4. gid_t st_gid - gid владельца
	5. off_t st_size - размер файла
	6. time_t st_atime - время последнего доступа к файлу
	7. time_t st_mtime - время последнео изменения файла
	8. time_t st_ctime - время последнего изменения атрибутов файла.
	
struct passwd *getpwuid(uid_t uid);
	1. uid - uid пользователя
	2. Возвращает указатель на struct passwd при успехе, иначе NULL и устанавливает errno {
		- ENOMEM - недостаточно памяти для struct passwd
		- EIO - ошибка ввода/вывода
		- EMFILE - достигнуто максимальное количество открытых файлов в процессе
		- ENFILE - достигнуто максимальное количество открытых файлов в системе
	} 
Функция getpwuid возвращает запись из /etc/passwd о переданном пользователе в виде struct passwd. 
В struct passwd хранится:
	1. char *pw_name - имя пользователя
    2. char *pw_passwd - пароль пользователя
    3. uid_t pw_uid - uid пользователя
	4. gid_t pw_gid - uid группы
	5. char *pw_dir - домашний каталог.
	
struct group *getgrgid(gid_t gid); 
	1. gid - gid группы пользователей
	2. Возвращает указатель на struct group при успехе, иначе NULL и устанавливает errno {
		- ENOMEM - недостаточно памяти для struct group
		- EIO - ошибка ввода/вывода
		- EMFILE - достигнуто максимальное количество открытых файлов в процессе
		- ENFILE - достигнуто максимальное количество открытых файлов в системе
	}  
Функция getgrgid возвращает запись из /etc/group о переданном пользователе в виде struct group. 
В struct group хранится:
	1. char *gr_name - название группы
	2. char *gr_passwd - пароль группы
	3. gid_t gr_gid - gid группы
	4. char **gr_mem - члены группы.
	
struct tm *localtime(const time_t *clock);
	1. clock - количество секунд, прошедших с 01.01.1970
	2. Возвращает указатель на struct tm в случае успеха, иначе NULL и устанавливает errno {
		- EOVERFLOW - результат не может быть представлен
	}
Функция localtime преобразует переданное количество прошедших секунд с 01.01.1970 в представление в виде struct tm.
В struct tm хранится:
	1. int tm_sec - секунды
	2. int tm_min - минуты
	3. int tm_hour - часы
	4. int tm_mday - день месяца
	5. int tm_mon - месяц
	6. int tm_year - год
	7. int tm_wday - день недели
	8. int tm_yday - день года
	9. int tm_isdst - летнее время.
	
size_t strftime(char *restrict s, size_t maxsize,
    const char *restrict format,
    const struct tm *restrict timeptr);
	1. s - выходная строка
	2. maxsize - максимальный размер выходной строки
	3. format - формат записи
	4. timeptr - указатель на struct tm, хранящую время
	5. Возвращает количество символов результирующей строки. Если количество символов превышает maxsize, возвращает 0.
Форматирует время, переданное в виде struct tm, в читаемую строку.
	
char *basename(char *path);
	1. path - имя файла вместе с путем
	2. Возвращает имя файла без пути к нему.
	
Макросы S_ISREG, S_ISDIR - определяют, является ли файл обычным или директорией.
Определения S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH и S_IXOTH служат для определения прав доступа файла.
*/

/*
Фукнция для вывода информации о типе и правах доступа файла

Аргументы: mode файла

Данная функция с помощью макросов определяет тип и права доступа файла и выводит соответствующие символы.
*/

void PrintFileMode(mode_t st_mode) {
	char mode[11];
	if(S_ISREG(st_mode)) mode[0] = '-';
	else if(S_ISDIR(st_mode)) mode[0] = 'd';
	else mode[0] = '?';
	mode[1] = (st_mode & S_IRUSR) ? 'r' : '-';
	mode[2] = (st_mode & S_IWUSR) ? 'w' : '-';
	mode[3] = (st_mode & S_IXUSR) ? 'x' : '-';
	mode[4] = (st_mode & S_IRGRP) ? 'r' : '-';
	mode[5] = (st_mode & S_IWGRP) ? 'w' : '-';
	mode[6] = (st_mode & S_IXGRP) ? 'x' : '-';
	mode[7] = (st_mode & S_IROTH) ? 'r' : '-';
	mode[8] = (st_mode & S_IWOTH) ? 'w' : '-';
	mode[9] = (st_mode & S_IXOTH) ? 'x' : '-';
	mode[10] = '\0';
	printf("%s", mode);
}

/*
Функция для вывода имени владельца файла

Аргументы: идентификатор пользователя

Данная функция получает запись /etc/passwd по переданному идентификатору и выводит полученное имя пользователя.
*/

int PrintUserName(uid_t st_uid) {
	struct passwd *getpwuidResult;
	getpwuidResult = getpwuid(st_uid);
	if(getpwuidResult == GETPWUID_ERROR) {
		perror("getpwuid error");
		return EXIT_FAILURE;
	}
	printf("\t%s", getpwuidResult->pw_name);
	free(getpwuidResult);
	return EXIT_SUCCESS;
}

/*
Функция для вывода имени группы файла

Аргументы: идентификатор группы

Данная функция получает запись /etc/group по переданному идентификатору и выводит полученное имя группы.
*/

int PrintGroupName(gid_t st_gid) {
	struct group *getgrgidResult;
	getgrgidResult = getgrgid(st_gid);
	if(getgrgidResult == GETGRGID_ERROR) {
		perror("getgrgid error");
		return EXIT_FAILURE;
	}
	printf("\t%s\t", getgrgidResult->gr_name);
	free(getgrgidResult);
	return EXIT_SUCCESS;
}

/*
Функция для вывода времени последнего изменения атрибутов файла

Аргументы: количество секунд, прошедших с 01.01.1970

Данная функция сначала преобразует количетсво секунд в struct tm с помощью localtime, далее преобразует полученный struct tm в читаемое представление в виде строки и выводит полученную строку.
*/

int PrintFileChangeTime(time_t ctime) {
	char formatedTime[DATE_MAX];
	struct tm *localtimeResult;
	int error;
	localtimeResult = localtime(&ctime);
	if(localtimeResult == LOCALTIME_ERROR) {
		perror("localtime error");
		return EXIT_FAILURE;
	}
	error = strftime(formatedTime, DATE_MAX, "%b %e %H:%M %Y", localtimeResult);
	if(error == STRFTIME_ERROR) {
		fprintf(stderr, "Error processing date in PrintFileAttributes\n");
		return EXIT_FAILURE;
	}
	printf("\t%s", formatedTime);
	return EXIT_SUCCESS;
}

/*
Функция для вывода информации о файле

Аргументы: имя файла

Данная функция выводит инйормацию о файле с помощью вышеописанных функций PrintFileMode, PrintUserName, PrintGroupName и PrintFileChangeTime.
*/

int PrintFileAttributes(char *name) {
	int error;
	struct stat fileStat;
	error = stat(name, &fileStat);
	if(error == STAT_ERROR) {
		perror("stat error");
		return EXIT_FAILURE;
	}
	PrintFileMode(fileStat.st_mode);
	error = PrintUserName(fileStat.st_uid);
	if(error == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	error = PrintGroupName(fileStat.st_gid);
	if(error == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if(S_ISREG(fileStat.st_mode)) {
		printf("%d", fileStat.st_size);
	}
	error = PrintFileChangeTime(fileStat.st_ctime);
	if(error == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	printf("\t%s\n", basename(name));
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	if(argc == NO_ARGS) {
		fprintf(stderr, "No arguments\n");
		return EXIT_FAILURE;
	}
	int i = 1, error;
	for(; i < argc; i++) {
		error = PrintFileAttributes(argv[i]);
		if(error == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
