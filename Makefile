CC=gcc
CFLAGS=-Wall -Wextra -g

all: test_file_utils

file_utils.o: file_utils.c file_utils.h
	$(CC) $(CFLAGS) -c file_utils.c

test_file_utils: test_file_utils.c file_utils.o
	$(CC) $(CFLAGS) -o test_file_utils test_file_utils.c file_utils.o

clean:
	rm -f *.o test_file_utils
