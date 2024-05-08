#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <ctype.h>

void wc(int mode, char* path){
	/*Feel free to change the templates as needed*/
	/*insert code here*/

	// file pointer to read to open file path
	FILE *file;

	// variables to stor word, line, and charater counts
	int words = 0,lines = 0,chars = 0;

	// pointer to track whether a word has been seen
	char ptr;

	// path is not NULL open the filepath
	if (path != NULL) {
		file = fopen(path, "r");
		if (file == NULL) {
			fprintf(stderr, "Error opening file\n");
			exit(1);
		}
	} else {
		// otherwise pipe in use, set input to stdin
		file = stdin;
	}

	// read in characters until end of file is reached
	while ((ptr = fgetc(file)) != EOF) {
		// increment number of characters in the file
		chars++;
		
		// if newline character is read, increment number of lines
		if (ptr == '\n') {
			lines++;
		}
		// if a space character is read move ptr to previous character
		if (isspace(ptr)) {
			ptr--;
			// if previous character is not a space then increment number of words
			if (!isspace(ptr)){
				words++;
			}
			// move the pointer back to current position
			ptr++;
		}
		ptr++;
	}
	// check for last word in the file since the file may not end in whitespace
	if (!isspace(ptr)) {
		words++;
	}
	
	// check which mode to print
	if (mode == 1) {
		printf("%d\n", lines);
	} else if (mode == 2) {
		printf("%d\n", words);
	} else if (mode == 3) {
		printf("%d\n", chars);
	} else {
		printf("%d\t%d\t%d\n", lines, words, chars);
	}

	// if path is not null then file exists, close the file
	if (path != NULL) {
		fclose(file);
	}
}

int main(int argc, char** argv){
	if(argc>2){
		if(strcmp(argv[1], "-l") == 0) { 
			wc(1, argv[2]);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, argv[2]);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, argv[2]);
		} else {
			printf("Invalid arguments\n");
		}
	} else if (argc==2){
	 	if(strcmp(argv[1], "-l") == 0) { 
			wc(1, NULL);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, NULL);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, NULL);
		} else {
    		wc(0, argv[1]);
    	}
  	} else {
  		wc(0,NULL);
  	}

	return 0;
}