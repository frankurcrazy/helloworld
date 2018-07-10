CC = gcc
CFLAGS = -Wall

all: npdsch_subframe npdsch_repetition

npdsch_subframe: npdsch_subframe.c
	$(CC) $(CFLAGS) $< -o $@

npdsch_repetition: npdsch_repetition.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f npdsch_subframe npdsch_repetition
