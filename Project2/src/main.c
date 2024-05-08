//main.c program to translate the virtual addresses from the virtual.txt
//into the physical frame address. Project 3 - CSci 4061

#include <stdio.h>
#include <stdlib.h>
#include "vmemory.h"

#define INPUT_FILE "../bin/virtual.txt"

int main(int argc, char* argv[])
{
	// Declare variables as necessary

	if (argc > 2) {
		printf("Too many arguments, enter up to one argument\n");
		exit(-1);
	}

	// "?" is a conditional operator that returns the second value if the first value is true
	int policy = (argc == 2) ? 1:0;
	initialize_vmanager(policy);

	// [Hint 1]: Use the bitwise operators and masking to extract the page number and offset
	// [Hint 2]: strtol() might be useful to convert the string to an integer
	
	// testing functionalality of TLB code below
	
	// Open the file
	FILE *input_file = fopen(INPUT_FILE, "r");
    if (input_file == NULL) {
        printf("Error opening input file!\n");
        return -1;
    }

    // Initialize virtual memory manager with FIFO policy
    initialize_vmanager(0);

    // Output TLB at the beginning
    print_tlb();
    // Translate all virtual addresses in INPUT_FILE
    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
		int p_addr;
        // Convert string to integer
        unsigned int v_add = (unsigned int) strtol(line, NULL, 16);
        // Translate virtual address to physical address
		int lookup = (v_add >> 12) & 0xFFFFF;
		int offset = v_add & 0xFFF;
		int frame = get_tlb_entry(lookup);
		if (frame == -1) {
        	p_addr = translate_virtual_address(v_add);
		} else {
			p_addr = frame << 12 | offset;
		}
        // Print physical address
        print_physical_address(p_addr, offset);
    }

    // Close the input file
    fclose(input_file);
    // Print hit rate of cache
    printf("Hit rate of the cache is %f\n", get_hit_ratio());
    // Print the TLB here
    print_tlb();

	free_resources(); // you don't need to worry about this function
	return 0;
}
