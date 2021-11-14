CFLAGS=-Wall -Wextra -ggdb -pedantic -O3 -std=c99
main: main.c
		$(CC) $(CFLAGS) -o main main.c