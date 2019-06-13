CC = gcc
CFLAGS = -Wall

all: band_FR1 band_FR2

band_RF1: band_FR1.c
	$(CC) $(CFLAGS) $< -o $@

band_RF2: band_FR2.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f band_FR1 band_FR2
