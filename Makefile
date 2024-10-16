CC = gcc

CFLAGS = -Wall -g -fpic

LDFLAGS =

liblwp.so: lwp.o
	$(CC) $(CFLAGS) -shared -o $@ lwp.o

myprog: lwp.o schedulers.o
	$(LD) $(LDFLAGS) -o myprog lwp.o schedulers.o

lwp.o: lwp.c
	$(CC) $(CFLAGS) -c -o lwp.o lwp.c

schedulers.o: schedulers.c
	$(CC) $(CFLAGS) -c -o schedulers.o schedulers.c

test: myprog
	echo Testing myprog
	./myprog < inputfile
	echo done.
