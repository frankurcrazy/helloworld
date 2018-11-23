CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall -lcrypto


APPS = dot16kdf hmac_digest short_hmac_digest cmac_digest cmac akid des_cbc

all: $(APPS)


dot16kdf: dot16kdf.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

hmac_digest: hmac_digest.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

short_hmac_digest: short_hmac_digest.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

cmac_digest: cmac_digest.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

cmac: cmac.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

akid: akid.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@

des_cbc: des_cbc.o utility.o
	$(CC) $(LDFLAGS) $^ -o $@


%.o: %.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APPS) *.o
