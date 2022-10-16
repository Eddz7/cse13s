CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic 

all: mathlib-test

mathlib-test: mathlib-test.o e.o newton.o bbp.o euler.o madhava.o viete.o
	$(CC) $(CFLAGS) -lm -o mathlib-test mathlib-test.o e.o newton.o bbp.o euler.o madhava.o viete.o

mathlib-test.o: mathlib-test.c mathlib.h 
	$(CC) $(CFLAGS) -c mathlib-test.c e.c newton.c bbp.c euler.c madhava.c viete.c

clean: 
	rm -f mathlib-test mathlib-test.o e.o newton.o bbp.o euler.o madhava.o viete.o  

format:
	clang-format -i -style=file *.[ch]
