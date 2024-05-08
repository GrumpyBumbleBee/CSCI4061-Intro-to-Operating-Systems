#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

    // Check if the number of arguments is valid
    int num_iters = 0;

    if (argc != 2) {
        printf("Wrong number of arguments.\n");
        printf("Usage: %s <num_iters>\n", argv[0]);
        return 1;
    }

    // Check if the argument is a non-negative integer
    if (argc == 2) {
        num_iters = atoi(argv[1]); // Convert string to integer
        if (num_iters < 0) {
            printf("Invalid argument.\n");
            return 1;
        }
    }

    printf("START pid: %d parent_pid: %d\n", getpid(), getppid());
    fflush(stdout); // Prevents duplicate I/O after a fork

    for (int i = 0; i < num_iters; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork() failed"); // An error occurred
            return 1;
        } 
        else if (pid > 0) { // Parent process,
            int stat;
            pid_t child_pid = waitpid(pid, &stat, 0); // get child pid
            if(child_pid == -1){        // go through this if failed
                // TODO: wait for this child process using its process id to terminate using waitpid()
                // also check for errors for waitpid
                perror("waitpid() failed");
                return 1;
            }
            if (WIFEXITED(stat)){   // Otherwise child exited normally
                printf("Exit status: %d\n", WEXITSTATUS(stat));
            }
            else if (WIFSIGNALED(stat)){    // Child exited because signal was not caught
                printf("Killed by signal: %d\n" , WTERMSIG(stat));
            }
        }
    }
    printf("FINISH pid: %d parent_pid: %d\n", getpid(), getppid());
    return 0;
}
