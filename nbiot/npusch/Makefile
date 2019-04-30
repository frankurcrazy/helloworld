CC = gcc
CFLAGS = -Wall

all: npusch_slot npusch_acknack

npusch_slot: npusch_slot.c
	$(CC) $(CFLAGS) $< -o $@

npusch_acknack: npusch_acknack.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f npusch_slot npusch_acknack
