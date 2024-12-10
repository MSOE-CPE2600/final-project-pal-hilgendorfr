# Assignment: Lab 13 - chat program
# File: Makefile
# Names: Ryan Pal Hilgendorf, Alexander Toma
# Created: 12/10/2024
CFLAGS=-c -g -Wall -Werror
CC=gcc

all: chatRead chatWrite

chatRead: chatRead.o
	$(CC) -o client chatRead.o

chatWrite: chatWrite.o
	$(CC) -o server chatWrite.o

clean:
	rm -f *.o server client

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

