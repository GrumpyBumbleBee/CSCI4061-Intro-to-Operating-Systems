#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

# define MAX_PATH 1024

// helper function to print mode
void print_mode_h(mode_t mode) {
    fprintf(stdout, "Mode: %o : ", mode);
    fprintf(stdout, (S_ISDIR(mode)) ? "d" : "-");
    fprintf(stdout, (mode & S_IRUSR) ? "r" : "-");
    fprintf(stdout, (mode & S_IWUSR) ? "w" : "-");
    fprintf(stdout, (mode & S_IXUSR) ? "x" : "-");
    fprintf(stdout, (mode & S_IRGRP) ? "r" : "-");
    fprintf(stdout, (mode & S_IWGRP) ? "w" : "-");
    fprintf(stdout, (mode & S_IXGRP) ? "x" : "-");
    fprintf(stdout,  (mode & S_IROTH) ? "r" : "-");
    fprintf(stdout, (mode & S_IWOTH) ? "w" : "-");
    fprintf(stdout, (mode & S_IXOTH) ? "x" : "-");
    fprintf(stdout, "\n");
}

void traverseDirectory(char *dirname){
    // TODO: change directory to dirname using chdir
    if(chdir(dirname) != 0){
        fprintf(stderr, "Cannot change directory to %s\n", dirname);
        exit(-1);
    }

    // open current direc
    DIR *dir = opendir("."); // remember we changed working directory using chdir
    if(!dir){
        fprintf(stderr, "Cannot open directory %s\n", dirname);
        exit(-1);
    }

    struct dirent *dir_entry;
    char *dir_entry_name;
    while((dir_entry = readdir(dir)) != NULL){
        
        // skip "." and ".."
        // "." denotes current directory and ".." denotes parent directory
        dir_entry_name = dir_entry->d_name;
        if(!strcmp(dir_entry_name, ".") || !strcmp(dir_entry_name, "..")){
            continue;
        }

        if(dir_entry->d_type == DT_DIR){ // directory
            fprintf(stdout, "Directory found: %s\n", dir_entry_name);
            }
        
        else { 
            struct stat stat_info;
            // TODO: run lstat(???) on the entry (dir_entry_name) to get stat_info
            if(lstat(dir_entry_name, &stat_info) != 0){
                fprintf(stderr, "Cannot run stat on %s\n", dir_entry_name);
                exit(-1);
            }
            
            if(dir_entry->d_type == DT_REG){ // regular file
                fprintf(stdout, "\nFile found: %s\n", dir_entry_name);
                print_mode_h(stat_info.st_mode);
                fprintf(stdout, "User ID of Owner: %d\n", stat_info.st_uid);
                fprintf(stdout, "Inode Number: %lu\n\n", (unsigned long)stat_info.st_ino);
            }
            
            // TODO: check for symbolic link
            // Hint: use dir_entry->d_type or S_ISLNK(???)
            if(S_ISLNK(stat_info.st_mode)){
                fprintf(stdout, "Symlink found: %s\n", dir_entry_name); }
        }
    }
    closedir(dir);
}

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Please provide the path!");
		exit(1);
	}

    // filenames
    char *temp_f = "linkfile.txt";
    char *hardlink_f = "dir0/hardlink.txt";
    char *symlink_f = "dir0/symlink.txt";

    // TODO: create hard link hardlink_f ("dir0/hardlink.txt") to temp_f ("linkfile.txt")

    if(link(temp_f, hardlink_f) != 0){ 
        fprintf(stderr, "Unable to create hard link %s\n", hardlink_f);
    }

    // TODO: create symbolic link symlink_f ("dir0/symlink.txt") to temp_f ("linkfile.txt")
    if(symlink(temp_f, symlink_f) != 0){ 
        fprintf(stderr, "Unable to create symbolic link %s\n", symlink_f);
    }

    printf("Stats for %s\n", argv[1]);

    char working_dir[MAX_PATH];
    sprintf(working_dir, "%s/%s", getcwd(NULL, 0), argv[1]); // get absolute path of current directory
	traverseDirectory(working_dir); // call traverseDirectory with working_dir
    return 0;
}
