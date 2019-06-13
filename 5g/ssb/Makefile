CC = gcc
CFLAGS = -Wall

all: time_loc freq_loc sync_raster

time_loc: time_loc.c
	$(CC) $(CFLAGS) $< -o $@

freq_loc: freq_loc.c
	$(CC) $(CFLAGS) $< -o $@

sync_raster: sync_raster.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f time_loc freq_loc sync_raster
