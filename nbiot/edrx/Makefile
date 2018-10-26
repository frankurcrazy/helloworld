CC = gcc

all: paging hash_id

paging: paging.o util.o
	$(CC) -Wall $^ -o $@

hash_id: hash_id.o util.o
	$(CC) -Wall $^ -o $@

%.o: %.c util.h
	$(CC) -Wall -c $<

clean:
	rm -f paging hash_id *.o
