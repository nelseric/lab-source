#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

static void handler(int sig, siginfo_t *si, void *unused){
    switch(sig){
        case SIGINT:
            printf("Exiting %d\n", getpid());
            exit(1);
            break;
        case SIGUSR1:
            printf("SIGUSR1 from %d\n", si->si_pid);
            break;
        case SIGUSR2:
            printf("SIGUSR2 from %d\n", si->si_pid);
            break;
    }
}

int main(int argc, char *argv[]){
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = handler;

    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("sigaction SIGINT");
        exit(1);
    }
    if(sigaction(SIGUSR1, &sa, NULL) == -1){
        perror("sigaction USR1");
        exit(1);
    }
    if(sigaction(SIGUSR2, &sa, NULL) == -1){
        perror("sigaction USR2");
        exit(1);
    }
    pid_t child1, child2;

    if((child1 = fork()) == -1){
        perror("c1 fork");
        exit(1);
    } else if(child1 == 0){
        //Child 1
        srand(1);
        while(1){
            int n = rand() % 5 + 1;
            sleep(n);
            kill(getppid(), SIGUSR1);
        }
    } else {
        if((child2 = fork()) == -1){
            perror("c2 fork");
            exit(1);
        } else if(child2 == 0){
            //Child 2
            srand(2);
            while(1){
                int n = rand() % 5 + 1;
                sleep(n);
                kill(getppid(), SIGUSR2);
            }
        } else {
            //Parent
            while(1)
                pause();
        }
    }

    return 0;
}



