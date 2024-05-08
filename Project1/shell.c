#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "util.h"

// helper function to run prompt at the command line
void command_prompt() {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("[4061-shell]%s $ ", cwd);
	} else {
		perror("getcwd() error");
		exit(EXIT_FAILURE);
	}
}

// helper function to execute commands
void exec_command(char *command, char *args[]) {
	pid_t pid;

	if ((pid = fork()) == -1) {
		perror("fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		execvp(command, args);
		perror("Command error");
		exit(EXIT_FAILURE);
	} else {
		int status;
		waitpid(pid, &status,0);
	}
}

void exec_redirection(char *command, char *args[], char *output, bool append) {
	pid_t pid;

	if ((pid = fork()) == -1) {
		perror("fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {

		if (output != NULL) {
			FILE *stream;
			if (append) {
				stream = fopen(output, "a");
			} else {
				stream = fopen(output, "w");
			}
			if (stream == NULL) {
				perror("fopen faild");
				exit(EXIT_FAILURE);
			}

			int fd = fileno(stream);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		execvp(command, args);
		perror("Command error");
		exit(EXIT_FAILURE);
	} else {
		int status;
		waitpid(pid, &status,0);
	}
}

void exec_pipe(char *commands[]){
	int fd[2];

	// create the pipe using pipe() function and fd[2] array above
	int ret = pipe(fd);

	if(ret == -1){
		printf("Error creating pipe...\n");
		exit(-1);
	}

	pid_t pid;

	if ((pid = fork()) == -1) {
		perror("fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) { //child
        int parent_out_fd;
		char input_str[1024];
        
        // close write end
        close(fd[1]);

        read(fd[0], &parent_out_fd, sizeof(int));
		FILE *input = fdopen(parent_out_fd, "r");
		// Line 100 produces error with 'input' L initialization of 'char *' from incompatible pointer type 'File *'
		char *child_args[] = {commands[1], input, NULL};
		exec_command(commands[1], child_args);
	
        close(fd[0]);

	} else{ //parent
		// close read end
        close(fd[0]);
		FILE *out;
		int out_fd = fileno(out);
		dup2(out_fd, STDOUT_FILENO);
		fclose(out);
		char *parent_args[] = {commands[0], NULL};
		execvp(commands[0], parent_args);

        write(fd[1], &out_fd, sizeof(int));
        close(fd[1]);
        wait(NULL);
	}
}

int main(){
	/*write code here*/

	// execution loop to run the shell
	while (true) {
		command_prompt();
		// do logic here

		
		char input[1024]; // Input buffer
		char *tokens[64]; // Tokenized input
		// this is the logic
		fgets(input, sizeof(input), stdin);
        trimwhitespace(input); // Remove trailing newline

        // Tokenize input
        int arg_count = parse_line(input, tokens, " ");

        if (arg_count > 0) {
			// check for redirect opperators
			bool append = false;
			char *output_file = NULL;
			bool pipe_true = false;
			char *pipe_cmd[2];
		
			for (int i = 0; i < arg_count; i++) {
				if (strcmp(tokens[i], ">") == 0) {
					// redirect should overwrite

					// check bounds of tokens
					if (i + 1 < arg_count) {
						output_file = tokens[i + 1];
						append = false;
						// remove redirect operator from commands
						tokens[i] = NULL;
						break;
					} else {
						fprintf(stderr, "No output file supplied after '>'\n");
						continue;
					}	
				} else if (strcmp(tokens[i], ">>") == 0) {
					// redirect should append

					//check bounds
					if (i + 1 < arg_count) {
						output_file = tokens[i + 1];
						append = true;
						tokens[i] = NULL;
						break;
					} else {
						fprintf(stderr, "No output file supplied after '>>'\n");
						continue;
					}
				} else if (strcmp(tokens[i], "|") == 0) {
					//check bounds
					if (i + 1 < arg_count) {
						pipe_cmd[0] = tokens[i-1];
						pipe_cmd[1] = tokens[i+1];
						pipe_true = true;
						tokens[i] = NULL;
						break;
					} else {
						fprintf(stderr, "Invalid use of '|'\n");
						continue;
					}
					
				}
			}

            // Handle built-in commands
            enum command_type cmd_type = get_command_type(tokens[0]);
            switch (cmd_type) {
                case CD:
                    if (arg_count != 2) {
                        fprintf(stderr, "Usage: cd <directory>\n");
                    } else {
                        char *cd_args[] = {tokens[0], tokens[1], NULL}; // Pass arguments to custom cd program
                        exec_command("./cd", cd_args); // Execute custom cd program
                    }
                    break;
                 case LS:
                    // Check if the next token is "-R" for recursive ls
                    if (arg_count >= 2 && strcmp(tokens[1], "-R") == 0) {
                        char *ls_args[] = {tokens[0], tokens[1], tokens[2], NULL}; // Pass arguments to custom ls program
                        exec_command("./ls", ls_args); // Execute custom ls program
                    } else {
                        char *ls_args[] = {tokens[0], tokens[1], NULL}; // Pass arguments to custom ls program
                        exec_command("./ls", ls_args); // Execute custom ls program
                    }
					break;
				case WC:
					exec_command("./wc", tokens); // Execute custom wc program
                    break;
                case EXIT:
                    exit(EXIT_SUCCESS);
                    break;
                default:
					if(pipe_true){
						exec_pipe(pipe_cmd);
					}

					if (output_file != NULL) {
                    	exec_redirection(tokens[0], tokens, output_file, append);
                	} else {
                    // Execute other commands using fork and exec
                    	exec_command(tokens[0], tokens);
                    
            		}
					break;
        	}
		
		}
	}
}
