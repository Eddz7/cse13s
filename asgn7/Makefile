CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o bv.o node.o bf.o bst.o ht.o speck.o parser.o
	$(CC) $(CFLAGS) -o banhammer banhammer.o bv.o node.o bf.o bst.o ht.o speck.o parser.o

banhammer.o:
	$(CC) $(CFLAGS) -c banhammer.c bv.c node.c bf.c bst.c ht.c speck.c parser.c

clean:
	rm -f banhammer banhammer.o bv.o node.o bf.o bst.o ht.o speck.o parser.o

format:
	clang-format -i -style=file *.[ch]
