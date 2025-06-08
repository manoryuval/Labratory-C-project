CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: check

check: check.c
	$(CC) $(CFLAGS) -o check check.c

clean:
	rm -f check
