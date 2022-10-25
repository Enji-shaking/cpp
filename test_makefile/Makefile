# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
INSTDIR = bin

# ****************************************************
# Targets needed to bring the executable up to date

test1: main1.o run1.o
	$(CC) $(CFLAGS) -o test1 main1.o run1.o 

main1.o: 
	$(CC) $(CFLAGS) -c main.cpp -o main1.o

run1.o: 
	$(CC) $(CFLAGS) -c run.cpp -o run1.o

# testFileInDir
test2: main2.o run2.o
	$(CC) $(CFLAGS) -o test2 dir1/main2.o dir1/run2.o 

main2.o: 
	$(CC) $(CFLAGS) -c dir1/main.cpp -o dir1/main2.o

run2.o: 
	$(CC) $(CFLAGS) -c run.cpp -o dir1/run2.o

# testMakefileInAnother
test3: 
	(cd dir2; make test3)


clean:
	rm **/*.o test1 test2 **/test3