#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

bool matchRegex(struct dirent *p, char pattern[FILENAME_MAX]){

    size_t length = strlen(p->d_name);

    int patternIdx = 0;
    int fileNameIdx = 0;
    bool match = false;
    size_t patternLength = strlen(pattern);
    for (int i = 0; patternLength > i; i++) {
        if (pattern[i] == '/') {
            printf("Symbol / is not supported pattern\n");
            return false;
        }
    }

    for (fileNameIdx = 0; (length > fileNameIdx) && (patternLength > patternIdx); fileNameIdx++) {

            if (('?' != pattern[patternIdx]) && ('*' != pattern[patternIdx])) {
                if (pattern[patternIdx] != p->d_name[fileNameIdx]) {   // if letters do not match then break
                    break;
                }
                patternIdx++; // continue checkup if match

            } else if ('?' == pattern[patternIdx]) {         // ?
                patternIdx++;
            } else {                                         // *
                while (patternLength > patternIdx) {
                    patternIdx++;
                    if ('*' != pattern[patternIdx]) 
                        break;   // skip all the '*'
                }
                if (patternLength == patternIdx) {           // pattern ended with '*'
                    match = true;
                    break;
                }
                if (pattern[patternIdx] == '?') {
                    patternIdx++;
                    continue;
                }
                while (length > fileNameIdx) {
                    if (pattern[patternIdx] == p->d_name[fileNameIdx]) {
                        break;
                    }
                    fileNameIdx++;
                }
                patternIdx++;
            }
        }
        if (length == fileNameIdx) {
            while (patternLength > patternIdx) {
                if ('*' != pattern[patternIdx])
                    break;
                patternIdx++;
            }
            if (patternLength == patternIdx) {
                match = true;
            }
        }
        if (match) {
            printf("%s\n", p->d_name);
	}
	return true;
}

int main(int argc, char **argv) {

    // char pattern[FILENAME_MAX];
    char* default_args[] = { ".", NULL };
    struct dirent *p;
    DIR *dir;
    int matched = 0;

    // open current directory
    if ((dir = opendir(".")) == NULL) {
        perror("couldn't open '.'");
        return 0;
    }

    char** pattern = (1 < argc) ? argv : default_args;

    // printf("Enter pattern: ");
    // fgets(pattern, FILENAME_MAX, stdin);
    // size_t patternLength = strlen(pattern) - 1;
    // pattern[patternLength] = '\0';

    // for (int i = 0; patternLength > i; i++) {
    //     if (pattern[i] == '/') {
    //         printf("Symbol / is not supported pattern\n");
    //         return 0;
    //     }
    // }

    while ((p = readdir(dir)) != NULL) {
        
        matched += matchRegex(p, argv[1]);
    }

    if (!matched) {
        printf("%s did not matched any file in the current directory\n", argv[1]);
    }

    closedir(dir);
    return 0;
}


