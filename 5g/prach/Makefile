CC = gcc
CFLAGS = -Wall

all: preamble_format ssb_associate cyclic_shift

preamble_format: preamble_format.c
	$(CC) $(CFLAGS) $< -o $@

ssb_associate: ssb_associate.c
	$(CC) $(CFLAGS) -lm $< -o $@

cyclic_shift: cyclic_shift.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f preamble_format ssb_associate cyclic_shift
