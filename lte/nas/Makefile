CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall -lcrypto

APPS = eea1 eea2 eia1 eia2

all: $(APPS)

eea1: eea1.o crypto_eea.o crypto_snow_3g.o crypto_cmac.o crypto.o
	$(CC) $(LDFLAGS) $^ -o $@

eea2: eea2.o crypto_eea.o crypto_snow_3g.o crypto_cmac.o crypto.o
	$(CC) $(LDFLAGS) $^ -o $@

eia1: eia1.o crypto_eia.o crypto_snow_3g.o crypto_cmac.o crypto.o
	$(CC) $(LDFLAGS) $^ -o $@

eia2: eia2.o crypto_eia.o crypto_snow_3g.o crypto_cmac.o crypto.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c crypto.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(APPS)
	@rm -f *.o
