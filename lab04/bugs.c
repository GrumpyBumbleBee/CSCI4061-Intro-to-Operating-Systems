#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// To be modified
// Hint: The bounds of input are not checked, so the call to strcpy() may overrun the space allocated for buffer and write into other functions’ stack space, change the size of buffer to a proper size
void func2(char* input) {
	
    char buffer[strlen(input) + 1];
    strcpy(buffer, input);
    printf("%s\n", buffer);
}

// To be modified
// Hint: free dynamically allocated memory
void func1(char* input) {

    char *longstr = (char *) malloc(1000);
    char shortstr[20];
    strcpy(longstr, input);
    strcpy(shortstr, "Small String");
    func2(longstr);
    func2(shortstr);
    free(longstr);
}

// Do not modify this part
int main() {
    char inp[1000];
    printf("Input string: ");
    fgets(inp, 1000, stdin);
    while (1) {
        func1(inp);
    }
    return 0;
}
