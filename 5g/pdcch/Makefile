CC = gcc
CFLAGS = -Wall

all: type0-PDCCH CCS0_pattern1_FR1 CCS0_pattern1_FR2 occasion 

type0-PDCCH: type0-PDCCH.c
	$(CC) $(CFLAGS) $< -o $@

CCS0_pattern1_FR1: CCS0_pattern1_FR1.c
	$(CC) $(CFLAGS) $< -o $@

CCS0_pattern1_FR2: CCS0_pattern1_FR2.c
	$(CC) $(CFLAGS) $< -o $@

occasion: occasion.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f type0-PDCCH
	rm -f CCS0_pattern1_FR1 CCS0_pattern1_FR2
	rm -f occasion 
