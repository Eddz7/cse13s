#Credit TA Sloan
CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic $(shell pkg-config --cflags gmp)
LDFLAGS = $(shell pkg-config --libs gmp)
COMMON_OBJS = randstate.o numtheory.o rsa.o 

all: keygen encrypt decrypt

keygen: keygen.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

encrypt: encrypt.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

decrypt: decrypt.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f keygen decrypt encrypt *.o

format:
	clang-format -i -style=file *.[ch]
 
