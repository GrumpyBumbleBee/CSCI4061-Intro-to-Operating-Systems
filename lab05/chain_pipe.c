/*
Check p3.c and p4.c
All the required code for the activity is present in the mentioned samples
You will have to work a bit more to figure out certain small details
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// writing only, create if file doesn't exist, clear the content if the file already exists.
#define WRITE (O_WRONLY | O_CREAT | O_TRUNC)
// read permission for the file owner, wite permission for the file owner
#define PERM (S_IRUSR | S_IWUSR)

int main(int argc, char *argv[]){
    // accept single argument and convert to integer n. n is number of processes
    // atoi stands for ascii to integer
    int n = atoi(argv[1]);

    pid_t pid;
    char data_from_parent[1024] = "";
    for(int i = 1; i < n; i++){
        int fd[2];
        // create the pipe using pipe() function and fd[2] array above
        pipe(fd);

        pid = fork();

        if(pid != 0){ // parent
            // close read end of pipe
            close(fd[0]);
            
            // write pid to the write end of pipe
            int id = getpid();
            write(fd[1], &id, sizeof(id));
            // close write end
            close(fd[1]);

            wait(NULL);
            break;
            
        } else if(pid == 0){// child
            pid_t pid_from_parent;
            char str_pid[10] = "";

            // close write end
            close(fd[1]);

            // read pid from read end of pipe and store it into `data_from_parent` buffer
            read(fd[0], &pid_from_parent, sizeof(pid_from_parent));
            sprintf(str_pid, "%d", pid_from_parent);
            strcat(data_from_parent, str_pid);
            strcat(data_from_parent, " ");

            // close read end
            close(fd[0]);

            if(i == n - 1){// If this is the last child process
                // Get current PID and store into the `data_from_parent` buffer
                int id = getpid();
                char str_id[10] = "";
                sprintf(str_id, "%d", id);
                strcat(data_from_parent, str_id);

                // STDOUT_FILENO is a macro that represents the file descriptor number of the standard output, usually terminal/console
                // Duplicate STDOUT_FILENO into TEMP_STDOUT so that we can resotre STDOUT_FILENO later
                int TEMP_STDOUT = dup(STDOUT_FILENO);

                // Open the out.txt file using permission given at the top
                int out_fd = open("out.txt", WRITE, PERM);

                // Redirect out_fd to STDOUT_FILENO
                int ret_val = dup2(out_fd, STDOUT_FILENO);

                // Print `data_from_parent`. Because we Redirect fd to STDOUT_FILENO, this should go to the out.txt
                printf("%s", data_from_parent);
                fflush(stdout); // this statement force the program output the data immediatly.

                // restore STDOUT_FILENO using TEMP_STDOUT
                ret_val = dup2(TEMP_STDOUT, STDOUT_FILENO);
                // This should go to the terminal
                printf("%s\n", data_from_parent);

                close(TEMP_STDOUT);
                close(out_fd);
            }
        }
    }
    return 0;
}