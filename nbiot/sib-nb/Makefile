CC = gcc
CFLAGS = -Wall

APPS = sib_sched sib1nb sib2nb

all: $(APPS)

sib_sched: sib_sched.o sib_common.o
	$(CC) $(CFLAGS) $^ -o $@

sib1nb: sib1nb.o sib_common.o
	$(CC) $(CFLAGS) $^ -o $@

sib2nb: sib2nb.o sib_common.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c sib_common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APPS) *.o
