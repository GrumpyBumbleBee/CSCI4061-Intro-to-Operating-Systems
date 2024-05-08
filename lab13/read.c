#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {

    int key = 1234;

    int shmid; // TODO: get shared memory id
    char *buf; // TODO: attach to shared memory

    // Get shared memory id
    shmid = shmget(key, SHM_SIZE, 0666);
    if(shmid == -1){
        perror("shmget");
        exit(1);
    }

    // Attach to shared memory
    buf = (char *)shmat(shmid, NULL, 0);
    if(buf == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
	// TODO: print shared memory, break if "exit" is reveived
        printf("%s", buf);
        if(strcmp(buf, "exit\n") == 0){
            break;
        }
        sleep(1);
    }

    shmdt(buf);
    
    return 0;
}
