CFLAGS=-c -g -Wall -Werror
CC=gcc

all: chatRead chatWrite

chatRead: chatRead.o
	$(CC) -o client chatRead.o

chatWrite: chatWrite.o
	$(CC) -o server chatWrite.o

clean:
	rm -f *.o chatRead chatWrite

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

