CC = gcc
CFLAGS = -Wall -lcrypto

APPS  = k_asme
APPS += k_enb
APPS += k_nas_enc
APPS += k_nas_int
APPS += k_nh
APPS += eea2_encrypt
APPS += eea2_decrypt
APPS += eia2_integrity


all: $(APPS)

k_asme: k_asme.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_enb: k_enb.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nas_enc: k_nas_enc.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nas_int: k_nas_int.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nh: k_nh.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

eea2_encrypt: eea2_encrypt.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

eea2_decrypt: eea2_decrypt.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

eia2_integrity: eia2_integrity.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@


%.o: %.c kdf.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f $(APPS) *.o
