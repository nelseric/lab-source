#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>

static void sig_handler(int signum, siginfo_t *si, void * nu){
    if(signum == SIGINT){
        exit(0);
    }
}

void * worker(void * arg){
    char * file = (char*) arg;
    sleep(10);
    puts(file);

    free(file);
    return NULL;
}

int main(int argc, char *argv[]){
    //main thread is dispatcher
    struct sigaction sa;
    sa.sa_sigaction = sig_handler;
    sa.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("SIGINT handler");
        exit(1);
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * 10);
    int active_threads = 0, max_threads = 10;
    while(1){
        char *file = readline("");
        if(active_threads == max_threads){
            max_threads = (max_threads * 3) / 2 + 5;
            threads = realloc(threads, sizeof(pthread_t) * max_threads);
        }
        if(pthread_create(threads + active_threads++, NULL, worker, file) == -1){
            perror("worker create");
            exit(1);
        }
    }
    return 0;
}
