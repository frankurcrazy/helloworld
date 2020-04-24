CC = gcc
CFLAGS = -Wall -lcrypto

APPS  = k_ausf
APPS += k_seaf
APPS += k_amf
APPS += k_gnb
APPS += k_nh
APPS += k_nas_enc
APPS += k_nas_int
APPS += k_rrc_enc
APPS += k_rrc_int
APPS += k_up_enc
APPS += k_up_int
APPS += xres_star
APPS += hxres_star


all: $(APPS)

k_ausf: k_ausf.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_seaf: k_seaf.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_amf: k_amf.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_gnb: k_gnb.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nh: k_nh.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nas_enc: k_nas_enc.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_nas_int: k_nas_int.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_rrc_enc: k_rrc_enc.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_rrc_int: k_rrc_int.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_up_enc: k_up_enc.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

k_up_int: k_up_int.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

xres_star: xres_star.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@

hxres_star: hxres_star.o kdf.o
	$(CC) $(CFLAGS) $^ -o $@


%.o: %.c kdf.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f $(APPS) *.o
