#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"

//global line-number counter to be sync'ed.
int line_ctr = 0;

//initialization of mutex locks
pthread_mutex_t line_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t seq_lock = PTHREAD_MUTEX_INITIALIZER;
//at least two mutex locks should be used

extern node *head;
//start of the threads
void* process_file(void *param)
{
	// cast parameter to string (filename)
	char *fname = (char*)param;
    // Loop to read lines from file
    while (1) {
	// Locking line counter to ensure access
        pthread_mutex_lock(&line_lock);
	// Read current line number and increment global line count
        int current_line = line_ctr++;
	    
	// Release lock
        pthread_mutex_unlock(&line_lock);

	// Read line from the file
        char *line = read_line(fname, current_line);
	    
	// Check if end of file reached
        if (line == NULL) {
            // nothing left to read
            break;
        }
	// Create new node with line info
        node *newnode = create_node(current_line, line);
	// Testing if making the node fails. (If does, free allocated memory and continue next iteration).
        if (newnode == NULL) {
            free(line);
            continue;
        }
	// Insert new node to linked list.
        insert(&head, newnode);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    FILE* in_ptr;
    int threads;
    char *filename = NULL;
    // Check if correct number of arguments provided.
    if (argc != 3) {
        printf("Incorrect arg count. Enter file name and thread count\n");
        exit(EXIT_FAILURE);
    }
    // Parse number of threads from command line argument
    threads = atoi(argv[2]);
    // Validate number of threads
    if (threads < 1 || threads > 16) {
        printf("Incorrect number of threads. Enter between 1 and 16\n");
        exit(EXIT_FAILURE);
    }
    // Get filename from command line arguments
    filename = argv[1];
    pthread_t thread_ids[threads]; 

    // Create threads to process the file
    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, process_file, filename) != 0) {
            perror("Failed to create thread");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < threads; i++) {
        if (pthread_join(thread_ids[i], NULL) != 0) {
            perror("Failed to join thread");
            return EXIT_FAILURE;
        }
    }
    // Traveerse linked list and output the content
    traversal(head);

    return 0;
}
