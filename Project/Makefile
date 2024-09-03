# makefile for scheduling program        
CC=gcc
DEPS = scheduler.h list.h process.h
OBJ = scheduler.o list.o
CFLAGS=-Wall
STD_FLAG=-std=c99

# Create the final executable by linking the object files
scheduler: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(STD_FLAG) -o scheduler

scheduler.o: scheduler.c $(DEPS)
	$(CC) $(CFLAGS) -c scheduler.c $(STD_FLAG)

list.o: list.c $(DEPS)
	$(CC) $(CFLAGS) -c list.c $(STD_FLAG)

clean:
	rm -rf *.o scheduler
