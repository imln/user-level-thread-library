#Author :Shuvojit
#Date: 8-11-2015
#make file for sthread library
CC = gcc
CFLAGS = -c -Wall

all: main

main: slimits sinclude sutils  squeue  ssync sthread
	$(CC) $(CFLAGS) main.c -o main.o
	$(CC) sutils.o squeue.o ssync.o sthread.o main.o -o main.out

slimits:
	$(CC) slimits.h

sinclude:
	$(CC) sinclude.h

sutils: sinclude
	$(CC) sutils.h
	$(CC) $(CFLAGS) sutils.c -o sutils.o

squeue: sinclude slimits
	$(CC) squeue.h
	$(CC) $(CFLAGS) squeue.c -o squeue.o

ssync: sinclude slimits sutils squeue 
	$(CC) ssync.h
	$(CC) $(CFLAGS) ssync.c -o ssync.o
	

sthread: sinclude slimits sutils squeue ssync
	$(CC) sthread.h
	$(CC) $(CFLAGS) sthread.c -o sthread.o

clean:
	rm *.o
	rm *.gch
	rm *.out