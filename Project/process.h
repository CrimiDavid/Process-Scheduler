/**
 *  process.h
 *
 *  Full Name: David Crimi  
 *  Course section: EECS 3221
 *  Representation of a process in the system.
 *  
 */


#ifndef PROCESS_H
#define PROCESS_H

// representation of a a process
typedef struct process {
    int  pid;
    int  cpu_time;
    int  io_time;  
    int  arrival_time;
    int  burst_time;
    int original_burst_time;
    int quant;
    int completion_time;
} Process;

#endif
