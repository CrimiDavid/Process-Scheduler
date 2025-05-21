# Process Scheduler

## Overview
This program simulates three different CPU scheduling algorithms:

- **First Come First Served (FCFS)**
- **Round Robin (RR)**
- **Shortest Job First (SJF)**

The scheduler tracks processes through their lifecycle from arrival to completion, including handling CPU bursts and I/O operations. The program outputs detailed information about process states at each time unit and provides performance metrics after execution completes.

### Author
**Name**: David Crimi  
**Course**: EECS 3221

---

## Features
- Process management through multiple queues (ready, running, blocked, finished)
- Three scheduling algorithms
- CPU utilization calculation
- Turnaround time calculation for each process
- Detailed timeline output showing process states at each time unit

---

## File Structure
- `scheduler.c` - Main program implementation  
- `list.h` - Queue implementation header  
- `scheduler.h` - Scheduler function declarations  
- `process.h` - Process structure definitions  

---

## Algorithms Implemented

### 1. First Come First Served (FCFS)
- Non-preemptive scheduling algorithm  
- Processes are executed in order of arrival  
- Once a process begins running, it continues until it completes its CPU burst  

### 2. Round Robin (RR)
- Preemptive scheduling algorithm  
- Each process is given a fixed time quantum (**2 time units**)  
- If a process doesn't complete within its time quantum, it's moved back to the ready queue  
- Provides fair execution time to all processes  

### 3. Shortest Job First (SJF)
- Non-preemptive scheduling algorithm  
- Selects the process with the shortest burst time  
- Tends to minimize average waiting time but can lead to starvation of longer processes  

---

## Compilation and Usage
Compile the program using:

```bash
gcc -o scheduler scheduler.c
```
Once compiled, the program can be executed as follows:

```bash
./scheduler <input_file> <algorithm>
```
