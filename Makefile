CC			= gcc
CFLAGS	= -Wall -Werror -g -fpic

all: minls minget

minls: minfs.o minls.o minprint.o
	$(CC) $(CFLAGS) -o minls minls.o minfs.o minprint.o

minget: minfs.o minget.o minprint.o
	$(CC) $(CFLAGS) -o minget minget.o minfs.o minprint.o

minls.o: minls.c minfs.h
	$(CC) $(CFLAGS) -c minls.c

minget.o: minget.c minfs.h
	$(CC) $(CFLAGS) -c minget.c

minfs.o: minfs.c minfs.h
	$(CC) $(CFLAGS) -c minfs.c

minprint.o: minprint.c minfs.h
	$(CC) $(CFLAGS) -c minprint.c

test: test.o
	$(CC) $(CFLAGS) -o test test.o

test.o: test.c minfs.h
	$(CC) $(CLFAGS) -c test.c

clean:
	rm -f minfs.o test test.o minls minget minls.o minget.o minprint.o
