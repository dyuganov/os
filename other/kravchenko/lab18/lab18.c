#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>

#define MAX_TIME_LENGTH 200
#define LSTAT_ERROR -1
#define PRINT_INFO_ERROR -1
#define CHECK_INIT -1
const char* default_args[] = { ".", NULL };

void print_directory_info(char* file_name, struct stat* file_stat)
{
    char mode[11];
    char lbuf[MAX_TIME_LENGTH] = { 0 };

    struct passwd* owner = getpwuid(file_stat->st_uid);
    struct group* owner_group = getgrgid(file_stat->st_gid);

    //checkting file type
    if (S_ISDIR(file_stat->st_mode))
    {
        mode[0] = 'd';
    }
    else if (S_ISREG(file_stat->st_mode))
    {
        mode[0] = '-';
    }
    else
    {
        mode[0] = '?';
    }

    //filling file rights
    // user
    mode[1] = (file_stat->st_mode & S_IRUSR) ? 'r' : '-';
    mode[2] = (file_stat->st_mode & S_IWUSR) ? 'w' : '-';
    mode[3] = (file_stat->st_mode & S_IXUSR) ? 'x' : '-';

    // group
    mode[4] = (file_stat->st_mode & S_IRGRP) ? 'r' : '-';
    mode[5] = (file_stat->st_mode & S_IWGRP) ? 'w' : '-';
    mode[6] = (file_stat->st_mode & S_IXGRP) ? 'x' : '-';

    // others
    mode[7] = (file_stat->st_mode & S_IROTH) ? 'r' : '-';
    mode[8] = (file_stat->st_mode & S_IWOTH) ? 'w' : '-';
    mode[9] = (file_stat->st_mode & S_IXOTH) ? 'x' : '-';

    mode[10] = '\0';

    // output long file statistics
    printf("%s", mode);
    printf("\t%d", file_stat->st_nlink); //кол-во жестких ссылок

    if (owner == NULL) 
    {
        printf("\t%d", file_stat->st_uid);
    } 
    else 
    {
        printf("\t%s", owner->pw_name);
    }

    if (owner_group == NULL) 
    {
        printf("\t%d", file_stat->st_gid);
    } 
    else 
    {
        printf("\t%s", owner_group->gr_name);
    }

    // size, date & name
    strftime(lbuf, MAX_TIME_LENGTH, "%H:%M %e %b %Y", localtime(&(file_stat->st_ctime))); //время последнего изменения
    printf("\t%lu\t%s\t%s\n", file_stat->st_size, lbuf, basename(file_name));
  
}

int main(int argc, char** argv)
{
    struct stat file_stat;
    char** files_in_directory;

    if (argc < 2) 
    {
        files_in_directory = (char**)default_args;
    } else 
    {
        files_in_directory = argv + 1;
    }

    int lstat_check = CHECK_INIT;
    for (int i = 0; files_in_directory[i] != NULL; i++) 
    {
        lstat_check = lstat(files_in_directory[i], &file_stat);
        if (lstat_check == LSTAT_ERROR) 
        {
            perror(files_in_directory[i]);
        }

        print_directory_info(files_in_directory[i], &file_stat);
    }
    return EXIT_SUCCESS;
}