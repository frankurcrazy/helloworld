CC = gcc
CFLAGS = -Wall

all: preamble_format ssb_associate

preamble_format: preamble_format.c
	$(CC) $(CFLAGS) $< -o $@

ssb_associate: ssb_associate.c
	$(CC) $(CFLAGS) -lm $< -o $@

#sync_raster: sync_raster.c
#	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f preamble_format ssb_associate
