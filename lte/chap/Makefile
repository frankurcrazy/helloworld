CC = gcc
CFLAGS = -Wall
LDFLAGS = -lcrypto

all: chap

chap: chap.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f chap
