#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {

    int key = 1234;

    int shmid; //TODO: get shared memory id
    char *buf; //TODO: attach shared memory

    // Get shared memory id
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if(shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    buf = (char *)shmat(shmid, NULL, 0);
    if(buf == (char *)-1) {
        perror("shmat");
        exit(1);
    }
    
    while (fgets(buf, SHM_SIZE, stdin) != NULL);

    shmdt(buf); // detach shared memory
    shmctl(shmid, IPC_RMID, 0); // destory
    
    return 0;
}
