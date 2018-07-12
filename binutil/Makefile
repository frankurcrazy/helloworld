CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall


APPS = binxor binfind bindiff bindump binedit bingen bincpy bincomb binmega


all: $(APPS)


binxor: binxor.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

binfind: binfind.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bindiff: bindiff.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bindump: bindump.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

binedit: binedit.o utility.o
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
	@rm -f $(APPS)
	@rm -f *.o

.PHONY: all clean
