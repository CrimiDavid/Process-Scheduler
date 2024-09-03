/**
 *  scheduler.c
 *
 *  Full Name: David Crimi
 *  Course section: EECS 3221
 *  Description of the program: This program uses 3 queues for scheduling, First come first served , Round robin , and Shorterst Job first. As well as 1 additional 
 *  queue "finish" to track and save process competions times.
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "scheduler.h"
#include "process.h"

#define SIZE 100


// ---------------------- Start of main  ----------------------

int main(int argc, char *argv[]) {
    Queue* q = createQueue();
    Queue* ready = createQueue();
    Queue* blocked = createQueue();
    Queue* running = createQueue();
    Queue* finish =createQueue();
    FILE *fp;
    int num_of_processes;

    fp = fopen(argv[1], "r");
fscanf(fp, "%d", &num_of_processes);    

if (num_of_processes > 0) {
    Process *processArr = malloc(num_of_processes * sizeof(Process));

    // Read all processes from the file
    for (int i=0; i<num_of_processes; i++) {
        fscanf(fp, "%d %d %d %d",
               &processArr[i].pid, 
               &processArr[i].cpu_time, 
               &processArr[i].io_time, 
               &processArr[i].arrival_time);
        processArr[i].burst_time = (processArr[i].cpu_time + 1) / 2;
        processArr[i].original_burst_time = processArr[i].burst_time; 
        processArr[i].quant = 2;  // Initialize Quantum to 2
    }
    // Sort the processes based on arrival_time and pid
    for (int i = 0; i < num_of_processes - 1; i++) {
        for (int j = i + 1; j < num_of_processes; j++) {
            if (processArr[i].arrival_time > processArr[j].arrival_time || 
                (processArr[i].arrival_time == processArr[j].arrival_time && processArr[i].pid > processArr[j].pid)) {
                Process temp = processArr[i];
                processArr[i] = processArr[j];
                processArr[j] = temp;
            }
        }
    }

    // Enqueue the sorted processes into the queue q
    for (int i=0; i<num_of_processes; i++) {
        enqueue(q, &processArr[i]);
    }                                                                               
}

// ---------------------- OUTPUT FILE Creation and naming ----------------------
if (argc < 3) {
    fprintf(stderr, "Usage: %s <input_file_name> <option>\n", argv[0]);
    exit(1);
}

char name[100];  // max filename size of 100.

// Find the position of the last period in argv[1]
char *dot = strrchr(argv[1], '.');
if (dot != NULL) {
    int lengthWithoutExtension = dot - argv[1];
    strncpy(name, argv[1], lengthWithoutExtension);
    name[lengthWithoutExtension] = '\0';  // Null-terminate the string
} else {
    strcpy(name, argv[1]);
}

// Concatenate the dash, argv[2], and .txt to the filename
snprintf(name + strlen(name), sizeof(name) - strlen(name), "-%s.txt", argv[2]);

FILE *outputFile;  // Declare a file pointer for the output file
outputFile = fopen(name, "w");  // Open the output file in write mode

if (outputFile == NULL) {  // check if the file opened successfully
    perror("Error opening output file");
    exit(1);
}


// ---------------------- First Come First Served Scheduling ----------------------
if(atoi(argv[2]) == 0){
int cpuActiveTime = 0;// time a process is active(in the running queue)
int time = 0;// total time the process is running for
while(q->front || running->front || blocked->front || ready->front) {
    fprintf(outputFile, "%d ", time);
    // Move processes from `q` to `ready` if their arrival time has come
    while(q->front != NULL && q->front->process->arrival_time <= time) {
        Process *arrived = dequeue(q);
        enqueue(ready, arrived);
    }

    // Transition from ready to running if running is empty
    if(running->front == NULL && ready->front != NULL) {
        Process *toRun = dequeue(ready);
        enqueue(running, toRun);
    }   

    // prints all process currently in the running queue
    if(ready->front != NULL){
    Node *current = ready->front;
    while(current != NULL){
    fprintf(outputFile, "%d:ready " , current->process->pid);
    current = current->next;
        }
    }

    // Handling Blocked Processes
    Node *current = blocked->front;
    while(current != NULL) {
        fprintf(outputFile, "%d:blocked ", current->process->pid);
        if(current->process->io_time > 0) {
        current->process->io_time--;// Decrement io_time for the current process
    }

    // If io_time is 0, move process to ready queue
    if(current->process->io_time == 0) {
        Process *toReady = dequeue(blocked);
        toReady->burst_time = toReady->original_burst_time;//reset the burst time
        enqueue(ready, toReady);
        current = blocked->front; // reset the current pointer to the front of the blocked queue
    } else {
        current = current->next; // If process is still blocked, move to the next process
    }
}


    // Handling Running Processes
    if(running->front != NULL) {
        cpuActiveTime++;
        if(running->front->process->cpu_time == 0) {
            dequeue(running);
            if (running->front == NULL) {
                continue;// skip the rest of the loop iteration
            } 
        }
        //while running process is greater than 0 decrement until burst is complete
        if(running->front->process->burst_time > 0) {
            fprintf(outputFile , "%d:running", running->front->process->pid);
            running->front->process->burst_time--;
        }
        //when burst is complete and process has i/o move to io queue
        if(running->front->process->burst_time == 0 && running->front->process->io_time > 0) {
            Process *done = dequeue(running);
            enqueue(blocked, done);
        }
        // Check if both burst_time and io_time are 0, indicating the process is done.
        if(running->front && running->front->process->burst_time == 0 && running->front->process->io_time == 0) {
            running->front->process->completion_time = time;  // Set completion time before dequeueing
            Process *finished = dequeue(running);
            enqueue(finish, finished);// add to finish queue for printing later 
        }
    }
    fprintf(outputFile, "\n");
    time++;
}
//printing of cpu utilization, finish time and turnaround times (SAME FOR RR and SJF)
float cpuUtilization = (float)cpuActiveTime / time;// calculate cpu utilization
int endTime = time - 1;
fprintf(outputFile , "\n");
fprintf(outputFile, "Finishing time: %d\nCPU utilization: %0.2f\n", endTime, cpuUtilization);
while(finish->front != NULL) {
    Process *finishedProcess = dequeue(finish);
    int turnaroundTime = (finishedProcess->completion_time - finishedProcess->arrival_time) + 1;
    fprintf(outputFile, "Turnaround Process %d: %d\n", finishedProcess->pid,turnaroundTime);
}

}




// ---------------------- Round Robin Scheduling ----------------------
if(atoi(argv[2]) == 1){

int cpuActiveTime = 0;
int time = 0;
while(q->front || running->front || blocked->front || ready->front) {
    fprintf(outputFile, "%d " , time );
    // Move processes from `q` to `ready` if their arrival time has come
    while(q->front != NULL && q->front->process->arrival_time <= time) {
        Process *arrived = dequeue(q);
        enqueue(ready, arrived);
    }

    // Transition from ready to running if running is empty
    if(running->front == NULL && ready->front != NULL) {
        Process *toRun = dequeue(ready);
        enqueue(running, toRun);
    }   
    
if (ready->front != NULL) {
    Node *current = ready->front;
    while (current != NULL) {
        fprintf(outputFile, "%d:ready ", current->process->pid);
        current = current->next;
    }
}


    // Handling Blocked Processes
Node *current = blocked->front;
while(current != NULL) {
    fprintf(outputFile, "%d:blocked ", current->process->pid);

    // Decrement io_time for the current process
    if(current->process->io_time > 0) {
        current->process->io_time--;
    }

    // If io_time is 0, move process to ready queue
    if(current->process->io_time == 0) {
        Process *toReady = dequeue(blocked);
        toReady->burst_time = toReady->original_burst_time;
        enqueue(ready, toReady);
        
        // Since dequeue modifies the queue structure, 
        // reset the current pointer to the front of the blocked queue
        current = blocked->front;
    } else {
        // If process is still blocked, move to the next process
        current = current->next;
    }
}


    // Handling Running Processes
if (running->front != NULL) {
    cpuActiveTime++;
    
    // Decrement quantum and burst_time for the current running process
    running->front->process->quant--;
    running->front->process->burst_time--;

    // Print the running process
    fprintf(outputFile, "%d:running", running->front->process->pid);

    // Check if the process has finished its CPU burst
    if (running->front->process->burst_time == 0) {
        if (running->front->process->io_time > 0) {
            // If process has IO time left, move it to the blocked queue
            Process *done = dequeue(running);
            enqueue(blocked, done);
        } else {
            // Else the process is completely done
            running->front->process->completion_time = time;  // Set completion time before dequeueing
            Process *finished = dequeue(running);
            enqueue(finish, finished);
;
        }
    } 
    // If quantum is exhausted before CPU burst is done, move the process back to ready queue
    else if (running->front->process->quant == 0) {
        Process *quantumMove = dequeue(running);
        quantumMove->quant = 2;  // Reset quantum
        enqueue(ready, quantumMove);
    }
}

    fprintf(outputFile, "\n");
    time++;
}
float cpuUtilization = (float)cpuActiveTime / time;
int endTime = time - 1;
fprintf(outputFile, "\n");
fprintf(outputFile, "Finishing time: %d\nCPU utilization: %0.2f\n", endTime, cpuUtilization);
// After your main scheduling loop and before fclose(fp)
while(finish->front != NULL) {
    Process *finishedProcess = dequeue(finish);
    int turnaroundTime = (finishedProcess->completion_time - finishedProcess->arrival_time) + 1;
    fprintf(outputFile , "Turnaround Process %d: %d\n", finishedProcess->pid,turnaroundTime);
}





}// RR close 





// ---------------------- Shortest-Job First ----------------------
if(atoi(argv[2]) == 2){
int cpuActiveTime = 0;
int time = 0;
while(q->front || running->front || blocked->front || ready->front) {
    fprintf(outputFile,"%d ", time);
    // Move processes from `q` to `ready` if their arrival time has come
    while(q->front != NULL && q->front->process->arrival_time <= time) {
        Process *arrived = dequeue(q);
        time_enqueue(ready,arrived);
    }


    // Transition from ready to running if running is empty
    if(running->front == NULL && ready->front != NULL) {
        Process *toRun = dequeue(ready);
        enqueue(running, toRun);
    }   
    
    if(ready->front != NULL){
    Node *current = ready->front;
    while(current != NULL){
    fprintf(outputFile,"%d:ready " , current->process->pid);
    current = current->next;
        }
    }

// Handling Blocked Processes
Node *current = blocked->front;
while(current != NULL) {
    fprintf(outputFile,"%d:blocked ", current->process->pid);

    // Decrement io_time for the current process
    if(current->process->io_time > 0) {
        current->process->io_time--;
    }

    // If io_time is 0, move process to ready queue
    if(current->process->io_time == 0) {
        Process *toReady = dequeue(blocked);
        toReady->burst_time = toReady->original_burst_time;
        time_enqueue(ready, toReady);
        
        // reset the current pointer to the front of the blocked queue
        current = blocked->front;
    } else {
        // If process is still blocked, move to the next process
        current = current->next;
    }
}


    // Handling Running Processes
    if(running->front != NULL) {
        cpuActiveTime++;
        if(running->front->process->cpu_time == 0) {
            dequeue(running);
            // Ensure there's a valid process in the queue before further processing
            if (running->front == NULL) continue; // skip the rest of the loop iteration
        }
        if(running->front->process->burst_time > 0) {
            fprintf(outputFile,"%d:running", running->front->process->pid);
            running->front->process->burst_time--;
        }
        if(running->front->process->burst_time == 0 && running->front->process->io_time > 0) {
            Process *done = dequeue(running);
            enqueue(blocked, done);
        }
        // Check if both burst_time and io_time are 0, indicating the process is done.
        if(running->front && running->front->process->burst_time == 0 && running->front->process->io_time == 0) {
            running->front->process->completion_time = time;  // Set completion time before dequeueing
            Process *finished = dequeue(running);
            enqueue(finish, finished);
        }
    }
    fprintf(outputFile,"\n");
    time++;
}//
float cpuUtilization = (float)cpuActiveTime / time;
int endTime = time - 1;
fprintf(outputFile,"\n");
fprintf(outputFile,"Finishing time: %d\nCPU utilization: %0.2f\n", endTime, cpuUtilization);
while(finish->front != NULL) {
    Process *finishedProcess = dequeue(finish);
    int turnaroundTime = (finishedProcess->completion_time - finishedProcess->arrival_time) + 1;
    fprintf(outputFile,"Turnaround Process %d: %d\n", finishedProcess->pid,turnaroundTime);
}
}



fclose(fp);
fclose(outputFile);
return 0;
}
