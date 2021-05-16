//
// Created by rey on 2/24/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>

char* default_args[] = { ".", NULL };

void option_list(char* name, struct stat* fs)
{
    char mode[11];
    char lbuf[FILENAME_MAX] = { 0 };
    struct passwd* uent = getpwuid(fs->st_uid);
    struct group* gent = getgrgid(fs->st_gid);
    char fname[FILENAME_MAX];

    // file mode
    if(S_IFREG == (fs->st_mode & S_IFMT))
        mode[0] = '-';
    else if (S_IFDIR == (fs->st_mode & S_IFMT))
        mode[0] = 'd';
    else
        mode[0] = '?';

    // user
    mode[1] = (fs->st_mode & S_IRUSR) ? 'r' : '-';
    mode[2] = (fs->st_mode & S_IWUSR) ? 'w' : '-';
    mode[3] = (fs->st_mode & S_IXUSR) ? 'x' : '-';

    // group
    mode[4] = (fs->st_mode & S_IRGRP) ? 'r' : '-';
    mode[5] = (fs->st_mode & S_IWGRP) ? 'w' : '-';
    mode[6] = (fs->st_mode & S_IXGRP) ? 'x' : '-';

    // others
    mode[7] = (fs->st_mode & S_IROTH) ? 'r' : '-';
    mode[8] = (fs->st_mode & S_IWOTH) ? 'w' : '-';
    mode[9] = (fs->st_mode & S_IXOTH) ? 'x' : '-';

    mode[10] = 0;
    printf("%s", mode);
    printf("\t%ld", fs->st_nlink);

    if (uent == NULL)
        printf("\t%d", fs->st_uid);
    else
        printf("\t%s", uent->pw_name);

    if (gent == NULL)
        printf("\t%d", fs->st_gid);
    else
        printf("\t%s", gent->gr_name);

    // size, date & name
    strftime(lbuf, FILENAME_MAX, "%H:%M %e %b %Y", localtime(&(fs->st_ctime)));
    printf("\t%lu\t%s\t%s\n", fs->st_size, lbuf, basename(name));
}

int main(int argc, char** argv)
{
    int i = 0;
    char** dirs = (1 < argc) ? (argv + 1) : default_args;
    for (i = 0; dirs[i] != NULL; i++)
    {
        struct stat fs;
        if(lstat(dirs[i], &fs) == 0)
            option_list(dirs[i], &fs);
        else
            perror(dirs[i]);
    }

}
