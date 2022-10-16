CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: sorting

sorting: sorting.o insert.o stats.o heap.o quick.o
	$(CC) $(CFLAGS) -lm -o sorting sorting.o insert.o stats.o shell.o heap.o quick.o  

sorting.o:
	$(CC) $(CFLAGS) -c sorting.c insert.c stats.c shell.c heap.c quick.c

clean:
	rm -f sorting sorting.o insert.o stats.o shell.o heap.o quick.o

format: 
	clang-format -i -style=file *.[ch]
