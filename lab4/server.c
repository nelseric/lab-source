#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>
#include <math.h>

int num_files = 0;
int total_time = 0;
pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;

static void sig_handler(int signum, siginfo_t *si, void * nu){
    if(signum == SIGINT){
        printf("\n%d files accessed, with %f(s) average access time.\n",
                num_files, (double)total_time / num_files);
        exit(0);
    }
}

void * worker(void * arg){
    char * file = (char*) arg;
    double num = (double)rand() / RAND_MAX;
    int twait = 10;
    if(num <= 0.8){
        twait = 1;
    } else {
        num = (double)rand() / RAND_MAX;
        twait = round(7.0 + 3.0 * num);
    }
    sleep(twait);
    printf("%s read in %d seconds\n",file,twait);

    pthread_mutex_lock(&stats_mutex);
    num_files++;
    total_time += twait;
    pthread_mutex_unlock(&stats_mutex);
    free(file);
    return NULL;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    //main thread is dispatcher
    struct sigaction sa;
    sa.sa_sigaction = sig_handler;
    sa.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("SIGINT handler");
        exit(1);
    }

    while(1){
        char *file = readline("> ");
        pthread_t newthread;
        if(pthread_create(&newthread, NULL, worker, file) == -1){
            perror("worker create");
            exit(1);
        }
        pthread_detach(newthread);
    }
    return 0;
    }
