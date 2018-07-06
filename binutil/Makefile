CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall


all: binfind bindiff bindump bingen bincpy bincomb binmega


binfind: binfind.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bindiff: bindiff.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bindump: bindump.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bingen: bingen.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bincpy: bincpy.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bincomb: bincomb.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

binmega: binmega.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@


%.o: %.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@rm -f binfind bindiff bindump bingen bincpy bincomb binmega
	@rm -f *.o

.PHONY: all clean
