CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

SRC = src/my_malloc.c
MAIN = tests/main.c

all:
	$(CC) $(CFLAGS) $(SRC) $(MAIN) -o test

run: all
	./test

clean:
	rm -f test