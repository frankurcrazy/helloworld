CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lgmp -lcrypto

all: pak

pak: pak.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f pak

.PHONY: pak clean
