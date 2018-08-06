CC = gcc
APPS = cbs_encode sms_encode sms_decode

all: $(APPS)

cbs_encode: cbs_encode.o gsm7bit.o
	$(CC) -Wall $^ -o $@

sms_encode: sms_encode.o gsm7bit.o
	$(CC) -Wall $^ -o $@

sms_decode: sms_decode.o gsm7bit.o
	$(CC) -Wall $^ -o $@

%.o: %.c gsm7bit.h
	$(CC) -Wall -c $<

clean:
	@rm -f $(APPS)
	@rm -f *.o
