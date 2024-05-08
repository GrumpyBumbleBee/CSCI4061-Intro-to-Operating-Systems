#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

void ls(char *path, bool recurse_flag) {
	/* Insert code here*/
	DIR *dirp;
	struct dirent *dp;
	struct stat statbuf;

	// if path not provided get working directory
	if (path == NULL) {
		// buffer to store current directory
		char cwd[1024];
		// get the current working directory and handle any errors
		 if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
		// set path to current working directory
		path = cwd;
	}

	// open directory stream with provided path and handle errors
	if ((dirp = opendir(path)) == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

	while ((dp = readdir(dirp)) != NULL) {
        // Ignore current directory (.) and parent directory (..)
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        // print out current file or directory name
        printf("%s\n", dp->d_name);

		// Get the current path and store information in statbuf
        char curr_path[1024];
        snprintf(curr_path, 1024, "%s/%s", path, dp->d_name);
		stat(curr_path, &statbuf);

        // Check if recursive flag is set and whether current path is a directory
        if (recurse_flag && S_ISDIR(statbuf.st_mode)) {
			// get new directory path for recursion
            char new_path[1024];
            snprintf(new_path, 1024, "%s/%s", path, dp->d_name);
			// recursively call ls on the new_path
            ls(new_path, true);
        }
    }
	// close the directory stream
    closedir(dirp);
}

int main(int argc, char *argv[]){
	if(argc < 2){ // No -R flag and no path name
		ls(NULL, false);
	} else if(strcmp(argv[1], "-R") == 0) { 
		if(argc == 2) { // only -R flag
			ls(NULL, true);
		} else { // -R flag with some path name
			ls(argv[2], true);
		}
	}else { // no -R flag but path name is given
    	ls(argv[1], false);
  }
	return 0;
}
