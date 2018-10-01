CC = gcc

all: downlink uplink

downlink: downlink.c
	$(CC) -Wall $< -o $@

uplink: uplink.c
	$(CC) -Wall $< -o $@

clean:
	rm -f downlink uplink
