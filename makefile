CFLAGS=-Wall -Wextra -ggdb -pedantic -O3 -std=c99
solver: solver.c
		$(CC) $(CFLAGS) -o solver solver.c