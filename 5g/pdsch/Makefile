CC = gcc
CFLAGS = -Wall

all: sliv dmrs ptrs tbs

sliv: sliv.c
	$(CC) $(CFLAGS) $< -o $@

dmrs: dmrs.c
	$(CC) $(CFLAGS) $< -o $@

ptrs: ptrs.c
	$(CC) $(CFLAGS) $< -o $@

tbs: tbs.c
	$(CC) $(CFLAGS) -lm $< -o $@

clean:
	rm -f sliv dmrs ptrs tbs
