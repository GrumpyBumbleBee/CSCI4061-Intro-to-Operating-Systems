#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "linked_list.h"
#include <sys/types.h>


// initialize head
node *head = NULL;

// sequence counter
int seq_ctr = 0;

// external mutix locks
extern pthread_mutex_t line_lock;
extern pthread_mutex_t list_lock;
extern pthread_mutex_t seq_lock;

//Read the file on a line by line basis
char* read_line(char* fname, int line_no) 
{
	// Open file
	FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		perror("Error opeing file");
		exit(EXIT_FAILURE);
	}

	char *line = NULL;
	size_t length = 0;
	ssize_t read;

	int current_line = 0;
	// Read each line until reaching specified line number
	while ((read = getline(&line, &length, fp)) != -1) {
		current_line++;
		// Test if current line number matches specified number: return if does.
		if (current_line == line_no) {
			fclose(fp);
			return line;
		}
	}

	fclose(fp);
	return NULL;
} 

//traverse the linked list
void traversal(node *head)
{
	// Starting from head of list
	node *current = head;
	while (current != NULL) {
		// Print sequence number, line number, and content of current node
		printf("%d, %d, %s\n", current->seq_no, current->line_no, current->content);
		// Move to next node
		current = current->next;
	}
}

// insert the node into the linked list
void insert(node **phead, node *newnode)
{
	// Lock mutex for accessing linked list
	pthread_mutex_lock(&list_lock);

	// If list empty or new node should be placed at the beginning:
	if (*phead == NULL || (*phead)->line_no > newnode->line_no) {
		// Set new node's next pointer to current head
		newnode->next = *phead;
		// And update head to point to new node
		*phead = newnode;
	}
	// Otherwise, traverse list to find correct position to insert the new node
	else {
		node *current = *phead;
		while (current->next != NULL && current->next->line_no < newnode->line_no) {
			current = current->next;
		}
		// set the next node to be the current node's next node.
		newnode->next = current->next;
		current->next = newnode;
	}
	// Unlocking mutex
	pthread_mutex_unlock(&list_lock);
}

//create a new node structure
node* create_node(int line_no, char *line)
{
	// Allocate memory for new node
	node *newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Failed to allocate memory for a new node.\n");
        return NULL;
    }

    // Lock the mutex for accessing seq_ctr
    pthread_mutex_lock(&seq_lock);
    // Assigning sequence number to new node and increment counter
    newNode->seq_no = seq_ctr++;
    // Unlocking mutex
    pthread_mutex_unlock(&seq_lock);

	// Assigning line number and content to new node created
    newNode->line_no = line_no;
    newNode->content = line;
    newNode->next = NULL;

    return newNode;
}
