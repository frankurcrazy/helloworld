CC = gcc
CFLAGS = -Wall

all: dmrs sliv

dmrs: dmrs.c
	$(CC) $(CFLAGS) $< -o $@

sliv: sliv.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f dmrs sliv 
