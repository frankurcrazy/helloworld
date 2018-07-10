CC = gcc
CFLAGS = -Wall

all: freq_domain time_domain

freq_domain: freq_domain.c
	$(CC) $(CFLAGS) $< -o $@

time_domain: time_domain.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f freq_domain time_domain
