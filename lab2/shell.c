#define _BSD_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char *argv[]){
    while(1){
        char * line = readline("$ ");
        char **args = calloc(strlen(line), sizeof(char*));
        int i = 0;
        if(strlen(line) > 0){
            args[0] = strtok(line, " ");
            while(args[i] != NULL){
                args[++i] = strtok(NULL, " ");
            }
            if(strcmp(args[0], "exit") == 0){
                break;
            }
            if(strcmp(args[0], "quit") == 0){
                break;
            }
            if(strcmp(args[0], "q") == 0){
                break;
            }

            pid_t pid;
            if((pid = fork()) == 0) {
                if(execvp(args[0], args) != 0){
                    perror("exec");
                    exit(1);
                }
            } else if(pid < 0) {
                perror("forking failed");
                exit(1);
            } else {
                //wait(NULL);
                struct rusage u;
                wait3(NULL, 0, &u);
                printf("%ld.%06ld(%ld.%06ld)\n",
                        u.ru_utime.tv_sec, u.ru_utime.tv_usec,
                        u.ru_stime.tv_sec, u.ru_stime.tv_usec);
            }
            free(args);
        }
        free(line);
    }
    return 0;
}
