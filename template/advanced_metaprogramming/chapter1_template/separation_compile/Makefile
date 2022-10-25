CC = g++
CFLAGS = -Wall -g

test1: sq.o
	$(CC) $(CFLAGS) -o test1 error.cpp sq.o

sq.o:
	$(CC) $(CFLAGS) -c sq.cpp

clean:
	rm -rf test* *.o