#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define INIT_CHECK 0
#define PUTENV_ERROR 1
#define EXEC_ERROR -1
#define END_OF_ENVP NULL

extern char** environ;

int execvpe(char* filename, char* argv[], char* envp[]){
        
    char** old_environ = environ;
    environ = envp;

    int execvp_check = INIT_CHECK;
    execvp_check = execvp(filename, argv);
    if (execvp_check == EXEC_ERROR)
    {
        perror("Execvpe error");
        environ = old_environ;
        return  EXEC_ERROR;
    }
}

int main(int argc, char* argv[], char* envp[]){
    
    if (argc != 2)
    {
        printf("Usage: a.out f1\n");
        return EXIT_FAILURE;
    }
  
    // int putenv_check = INIT_CHECK;
    // putenv_check = putenv("TZ=America/Los_Angeles");
    // if (putenv_check >= PUTENV_ERROR)
    // {
    //     perror("Putenv executed with error");
    //     return EXIT_FAILURE;
    // }  

    //char* spoiled_envp[2] = {"PATH=/home/students/19200/a.kravchenko2/lab11/spoiled_cat", END_OF_ENVP};
    char* spoiled_envp[2] = {"PATH=/home/students/19200/a.kravchenko2/lab11/dog", END_OF_ENVP};

    int execvpe_check = INIT_CHECK;
    //execvpe_check = execvpe(argv[1], &argv[1], envp);
    execvpe_check = execvpe(argv[1], &argv[1], spoiled_envp);
    
    if (execvpe_check == EXEC_ERROR)
    {
        perror("Execvpe executed with error");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
