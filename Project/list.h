/**
 *  list.h
 *
 *  Full Name: David Crimi  
 *  Course section: EECS 3221
 *  Description of the program: list data structure containing the tasks in the system
 *  
 */
 
#include "process.h"

typedef struct node {
    Process *process;
    struct node *next;
}Node;

typedef struct queue {
    Node *front;
    Node *rear;
} Queue;


void time_enqueue(Queue *q, Process *p);
void insert(struct node **head, Process *process);
void delete(struct node **head, Process *process);
void traverse(struct node *head);
void enqueue(Queue* q, Process* process);
Queue* createQueue();
Process* dequeue(Queue* q);