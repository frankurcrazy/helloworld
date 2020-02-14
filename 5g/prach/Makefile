CC = gcc
CFLAGS = -Wall

all: preamble_format ssb_associate_1 ssb_associate_2 cyclic_shift t_start

preamble_format: preamble_format.c
	$(CC) $(CFLAGS) $< -o $@

ssb_associate_1: ssb_associate_1.c
	$(CC) $(CFLAGS) -lm $< -o $@

ssb_associate_2: ssb_associate_2.c
	$(CC) $(CFLAGS) -lm $< -o $@

cyclic_shift: cyclic_shift.c
	$(CC) $(CFLAGS) $< -o $@

t_start: t_start.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f preamble_format ssb_associate_1 ssb_associate_2 cyclic_shift t_start
