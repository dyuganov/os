#include <stdio.h>
#include <stdlib.h>

extern char **environ;

void printEnvVariables(){
    printf("My environment variables:\n");
    for(char **envVar = environ; *envVar != NULL; ++envVar) {
        printf("%s\n", *envVar);
    }
}

void printArgs(const int argc, char **argv){
    const int MIN_NUM_OF_ARGS = 2;
    if (argc >= MIN_NUM_OF_ARGS) {
        printf("My arguments: \n");
        for (int arg_num = 1; arg_num < argc; arg_num++) {
            printf("%s\n", argv[arg_num]);
        }
    }
}

int main(int argc, char **argv) {
    printEnvVariables();
    printArgs(argc, argv);
    printf("End of program\n");
    return EXIT_SUCCESS;
}
