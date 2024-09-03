My program simulates a system scheduler. It reads a list of processes from a file and schedules them according to three different algorithms specifed by the user:

    0.First Come First Served (FCFS)
    1.Round Robin (RR) with a Quantum of 2
    2.Shortest Job First (SJF)

The program maintains five queues to handle the scheduling:

    Queue q: Holds processes read from the input file.
    Queue ready: Holds processes that are ready to execute but not currently executing.
    Queue blocked: Holds processes that are waiting for I/O operations to complete.
    Queue running: Contains the process that is currently executing.
    Queue finish: Stores the processes once they've completed execution, helping in determining their completion times.

After scheduling the processes, the program produces an output file detailing the state of each process at each time step.


Dependencies: 
Uses standard C libraries and 'list.h' , 'scheduler.h', 'process.h'.



List.c:

this file defines the queues used throught the project. The file provides functions to create a queue, add a process to the end of the queue, 
remove a process from the front of the queue, and a specialized function for the Shortest job first scheduler that orginizes the queue based on the CPU time of a process.

