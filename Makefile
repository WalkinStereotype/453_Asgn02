CC = gcc

CFLAGS = -Wall -g -fpic

LDFLAGS =

liblwp.so: lwp.o
	$(CC) $(CFLAGS) -shared -o $@ lwp.o magic64.o roundRobinSchedulers.o

lwp.o: lwp.c
	$(CC) $(CFLAGS) -c -o lwp.o lwp.c

magic64.o: magic64.S
	$(CC) $(CFALGS) -o magic64.o -c magic64.S

roundRobinSchedulers.o: roundRobinSchedulers.c
	$(CC) $(CFLAGS) -c -o roundRobinSchedulers.o roundRobinSchedulers.c

tests.o: tests.c
	$(CC) $(CFLAGS) -c -o tests.o tests.c

test: tests.o
	$(CC) $(CFLAGS) -o test tests.o
