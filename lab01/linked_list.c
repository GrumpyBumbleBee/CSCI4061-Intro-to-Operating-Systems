#include <stdio.h>
#include "linked_list.h"

// Head pointer for the linked list
Node *head = NULL;

Node *createNode(int value){
    Node *node =  (Node *)malloc(sizeof(Node));
    node->val = value;
    node->next = NULL;
    return node;
}

void insertLL(int value){
    // Cases to handle:
    // Empty linked list [Head is NULL]
    if(head == NULL){
        head = createNode(value);
    }

    // value < head->val
    else if (value < head-> val){
        Node *newNode = createNode(value);
        newNode->next = head;
        head = newNode;
    }
        
    // value > last node in linked list
    // insert value within linked list
    else {
        Node *current = head;
        while(current->next != NULL && value > current->next->val){
            current = current->next;
        }
        Node *newNode = createNode(value);
        newNode->next = current->next;
        current->next = newNode;
    }
}

void freeLL(){
    Node *nextNode = NULL;
    while(head){
        nextNode = head -> next;
        free(head);
        head = nextNode;
    }
    head = NULL;
}

void displayLL(){
    Node *trav = head;
    while(trav){
        printf("%d ", trav->val);
        trav = trav->next;
    }
    printf("\n");
}