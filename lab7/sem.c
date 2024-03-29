#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main (int argc, char *argv[])
{
    int status;
    long int i, loop, temp, *shmPtr;
    int shmId, semid;
    pid_t pid;

    // get value of loop variable (from command-line argument)
    if(argc >= 2){
        loop = atol(argv[1]);
    } else {
        puts("Invalid Usage");
        exit(1);
    }

    if((shmId = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    if((semid = semget(IPC_PRIVATE, 1, 00600)) == -1) {
        perror("semget");
        exit(1);
    }

    if(semctl(semid, 0, SETVAL, 1) == -1){
        perror("semctl setval 1");
        exit(1);
    }

    struct sembuf sem_wait = {0, -1, 0};

    struct sembuf sem_sig = {0, 1,0};

    shmPtr[0] = 0;
    shmPtr[1] = 1;

    if (!(pid = fork())) {
        for (i=0; i<loop; i++) {
            // swap the contents of shmPtr[0] and shmPtr[1]
            if(semop(semid, &sem_wait, 1) == -1){
                perror("sem_wait");
                exit(1);
            }
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
            if(semop(semid, &sem_sig, 1) == -1){
                perror("sem_wait");
                exit(1);
            }
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        exit(0);
    }
    else
        for (i=0; i<loop; i++) {
            // swap the contents of shmPtr[1] and shmPtr[0]
            if(semop(semid, &sem_wait, 1) == -1){
                perror("sem_wait");
                exit(1);
            }
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
            if(semop(semid, &sem_sig, 1) == -1){
                perror("sem_wait");
                exit(1);
            }
        }

    wait (&status);
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }

    if(semctl(semid, 0, IPC_RMID) == -1){
        perror("can't remove semaphore");
        exit(1);
    }

    return 0;
} 
