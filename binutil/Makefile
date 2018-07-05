CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall


all: binfind bindiff bingen bincpy bincomb binmega


binfind: binfind.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

binfind.o: binfind.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


bindiff: bindiff.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bindiff.o: bindiff.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


bingen: bingen.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bingen.o: bingen.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


bincpy: bincpy.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bincpy.o: bincpy.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


bincomb: bincomb.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

bincomb.o: bincomb.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


binmega: binmega.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

binmega.o: binmega.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@rm -f binfind bindiff bingen bincpy bincomb binmega
	@rm -f *.o

.PHONY: all clean
