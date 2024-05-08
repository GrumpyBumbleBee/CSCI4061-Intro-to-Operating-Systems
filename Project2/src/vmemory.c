//Implement the API modeling the translation of virtual page address to a
//physical frame address. We assume a 32 bit virtual memory and physical memory.
//Access to the page table is only via the CR3 register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vmemory.h"

#define OUT_TLB "../bin/tlb_out.txt"

// Global variables for TLB implementation
#define TLB_SIZE 8
int tlb[TLB_SIZE][3]; // changed to include frequency to TLB
int tlb_ptr = 0; // Pointer to the next available slot in TLB
int hit_count = 0;
int miss_count = 0;

bool FIFO_policy = true;
int **cr3;
//
// You can declare global variables necessary for your TLB implementation here
// TODO 1: Add global variables here as necessary
// TODO 2: Define the TLB with correct size and dimension
// [Hint]: You can use static variables like hit_count, miss_count, front or back for FIFO
//

static void initialize_tlb() {
    for (int i = 0; i < TLB_SIZE; i++) {
       if(FIFO_policy == false){
			populate_tlb_LFU(-1, -1);
		}else{
			populate_tlb(-1, -1);
		} 
    }
}
//
// More static helper functions can be implemented here
//


// The implementation of get_vpage_cr3 is provided in 
// an object file, so no need to re-implement it
void initialize_vmanager(int policy) 
{
	// Set LFU policy when passed as a parameter
	if (policy)
		FIFO_policy = false;
	// Set base pointer to page table
	cr3 = get_vpage_cr3();
	
	
	// You can add other initialization here as necessary
	initialize_tlb();
}

//
// The implementation of the following functions is required
//
int translate_virtual_address(unsigned int v_addr)
{
	// [Hint 1]: Use the bitwise operators to extract the page number and offset
	// [Hint 2]: Useful bitwise operators: >>, <<, &, |
	//TODO 1: return -1 if the second-level page table is NULL
	unsigned int first_level_index = (v_addr >> 22) & 0x3FF;
	unsigned int second_level_index = (v_addr >> 12) & 0x3FF;

	// Check if 2nd-level page table is NULL:
	if (cr3[first_level_index] == NULL) {
		populate_tlb(v_addr >> 12, -1); 	// populate_tlb even if translate_virtual_address returns -1
							// Meaning there is no corresponding entry in the page table.
		return -1;
	}

	// Extract physical from number from 2nd-level page table:
	int frame_num = cr3[first_level_index][second_level_index];

	// Check if frame number and offset get physical addr:
	if (frame_num == -1) {
		return -1;
	}

	// Combining frame num and offset to get physical addr:
	int offset = v_addr & 0xFFF;
	int p_addr = (frame_num << 12) | offset;

	// Populate TLB
    if(FIFO_policy == false){
		populate_tlb_LFU(v_addr >> 12, frame_num);
	}else{
		populate_tlb(v_addr >> 12, frame_num);
	}

	return p_addr;
}

void print_physical_address(int frame, int offset)
{
	//TODO : Print the physical address in the correct format
	if (frame == -1) {
		printf("-1\n");
	} else {
		printf("0x%08x\n", frame | offset);
	}
}

int get_tlb_entry(int n) {
    int v_page_number = n;
    // Search for the entry in the TLB
    for (int i = 0; i < TLB_SIZE; i++) {
		int v_addr = tlb[i][0];
        if (v_addr == v_page_number) { // Compare only the 20 MSB
            hit_count++;
            return tlb[i][1]; // Return the physical frame base address
        }
    }
    miss_count++;
    return -1; // TLB miss, return -1
}

void populate_tlb(int v_addr, int p_addr) {

    tlb[tlb_ptr][0] = v_addr;
    tlb[tlb_ptr][1] = p_addr;
    tlb_ptr = (tlb_ptr + 1) % TLB_SIZE;
}

void populate_tlb_LFU(int v_addr, int p_addr){
	for(int i = 0; i < TLB_SIZE; i++){
		// check if virtual adress is exists in TLB
		if(tlb[i][0] == v_addr){
			tlb[i][1] = p_addr;
			// increments the frequency of the entry
			tlb[i][2] += 1;
			return;
		}
	}

	// add new entry if TLB is not full
	if(tlb_ptr < TLB_SIZE){
		tlb[tlb_ptr][0] = v_addr;
    	tlb[tlb_ptr][1] = p_addr;
		tlb[tlb_ptr][2] = 1;
    	tlb_ptr = (tlb_ptr + 1) % TLB_SIZE;
	}else{ 
		// LFU if TLB is full
		int min_freq_index = 0;

		for(int i = 0; i < TLB_SIZE; i++){
			if(tlb[i][2] < tlb[min_freq_index][2]){
				min_freq_index = i;
			}
		}
		tlb[min_freq_index][0] = v_addr;
    	tlb[min_freq_index][1] = p_addr;
		tlb[min_freq_index][2] = 1;	
	}

}

// hit ratio = #hits / (#hits + #misses), # = total number of
float get_hit_ratio() {
    if (hit_count + miss_count == 0) {
        return 0.0;
    }
    return (float) hit_count / (hit_count + miss_count);
}

// Write to the file denoted by OUT_TLB
void print_tlb() {
    FILE *out_file = fopen(OUT_TLB, "a");
    if (out_file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < TLB_SIZE; i++) {
		if (tlb[i][0] == -1){
			fprintf(out_file, "%d %d\n", tlb[i][0], tlb[i][1]);
		} else {
        	fprintf(out_file, "0x%05x 0x%05x\n", tlb[i][0], tlb[i][1]);
		}
    }
    fprintf(out_file, "\n");

    fclose(out_file);
}
