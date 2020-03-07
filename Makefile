CC			= gcc
CFLAGS	= -Wall -Werror -g -fpic

all: minfs

minfs: minfs.o
	$(CC) $(CFLAGS) -o minfs minfs.o

minfs.o: minfs.c minfs.h
	$(CC) $(CFLAGS) -c minfs.c

test: test.o
	$(CC) $(CFLAGS) -o test test.o

test.o: test.c
	$(CC) $(CLFAGS) -c test.c

clean:
	rm -f minfs minfs.o test test.o
