CC = gcc


all: json_encode json_decode

json_encode: json_encode.c
	$(CC) -Wall -ljson $< -o $@

json_decode: json_decode.c
	$(CC) -Wall -ljson $< -o $@

clean:
	rm -f json_encode json_decode
