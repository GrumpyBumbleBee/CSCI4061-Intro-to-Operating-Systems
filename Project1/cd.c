#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>

	/**
	 * void cd():
	 * 		Used to stimulate changing the working directory to the given path "..."
	 * 		indicates the parent directory. 
	 *  	- Print the working directory after you perform the [0.5]cd operation.
	 *  	- List of test cases:
	 * 			./cd <path>
	 * 			./cd ..
	 *  	- Use [0.5]chdir system call. 
	 *  	- Supposed to change the current working directory inside the program, but DOES NOT 
	 * 	  	  change the working directory of current shell.
	*/

void cd(char* arg){
	// Change working directory to the given path
	chdir(arg);

	// Create buffer to store current working directory
	char cwd[1024];

	// Get current working directory
	getcwd(cwd, sizeof(cwd));

	// Print out the current working directory
	printf("Current working directory:  %s\n", cwd);
}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}