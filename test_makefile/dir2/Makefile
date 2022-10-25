# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

test3: main.o run.o
	$(CC) $(CFLAGS) -o test3 main.o run.o 

main.o: main.cpp run.h
	$(CC) $(CFLAGS) -c main.cpp

run.o: run.h
