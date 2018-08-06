CC = gcc
APPS = aka_network aka_user aka_reverse aka_auts

all: $(APPS)

aka_network: aka_network.o aka_f12345.o
	$(CC) -Wall $^ -o $@

aka_user: aka_user.o aka_f12345.o
	$(CC) -Wall $^ -o $@

aka_reverse: aka_reverse.o aka_f12345.o
	$(CC) -Wall $^ -o $@

aka_auts: aka_auts.o aka_f12345.o
	$(CC) -Wall $^ -o $@

%.o: %.c aka_f12345.h
	$(CC) -Wall -c $<

clean:
	@rm -f $(APPS)
	@rm -f *.o

.PHONY: all clean
