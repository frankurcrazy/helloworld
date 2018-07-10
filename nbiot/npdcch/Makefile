CC = gcc
CFLAGS = -Wall

all: npdcch_k0 npdcch_subframe

npdcch_subframe: npdcch_subframe.c npdcch_common.c
	$(CC) $(CFLAGS) $< -o $@

npdcch_k0: npdcch_k0.c npdcch_common.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f npdcch_k0 npdcch_subframe
