/**
 *  list.c
 *
 *  Full Name: David Crimi  
 *  Course section: EECS 3221
 *  Description of the program: Various list operations
 *  
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "process.h"



//Create a queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// add process and it contents to the back of the queue
void enqueue(Queue* q, Process* process) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->process = process;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;
        return;
    }

    q->rear->next = new_node;
    q->rear = new_node;
}

// remove process and its contents from the front of the queue
Process* dequeue(Queue* q) {
    if (q->front == NULL) {
        return NULL;
    }

    Node* temp = q->front;
    Process* removedProcess = temp->process;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return removedProcess;
}

// used for the SJF scheduler, function enqeues process based on cputime
void time_enqueue(Queue *q, Process *p) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->process = p;
    new_node->next = NULL;
    
    if(q->front == NULL) { // If the queue is empty
        q->front = new_node;
        q->rear = new_node;
    } else {
        Node *current = q->front;
        Node *prev = NULL;
        
        // Find the correct position to insert the process
        while(current != NULL && (current->process->cpu_time < p->cpu_time ||
               (current->process->cpu_time == p->cpu_time && current->process->pid < p->pid))) {
            prev = current;
            current = current->next;
        }
        
        if(prev == NULL) { // Insert at the beginning
            new_node->next = q->front;
            q->front = new_node;
        } else if(current == NULL) { // Insert at the end
            q->rear->next = new_node;
            q->rear = new_node;
        } else { // Insert in the middle
            new_node->next = current;
            prev->next = new_node;
        }
    }
}