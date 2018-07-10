CC = gcc
CFLAGS = -Wall

all: freq_dl freq_ul inband_dl inband_ul guardband_dl guardband_ul

freq_dl: freq_dl.o common.o
	$(CC) $(CFLAGS) $^ -o $@

freq_dl.o: freq_dl.c common.h
	$(CC) $(CFLAGS) -c $<

freq_ul: freq_ul.o common.o
	$(CC) $(CFLAGS) $^ -o $@

freq_ul.o: freq_ul.c common.h
	$(CC) $(CFLAGS) -c $<

inband_dl: inband_dl.o common.o
	$(CC) $(CFLAGS) $^ -o $@

inband_dl.o: inband_dl.c common.h
	$(CC) $(CFLAGS) -c $<

inband_ul: inband_ul.o common.o
	$(CC) $(CFLAGS) $^ -o $@

inband_ul.o: inband_ul.c common.h
	$(CC) $(CFLAGS) -c $<

guardband_dl: guardband_dl.o common.o
	$(CC) $(CFLAGS) $^ -o $@

guardband_dl.o: guardband_dl.c common.h
	$(CC) $(CFLAGS) -c $<

guardband_ul: guardband_ul.o common.o
	$(CC) $(CFLAGS) $^ -o $@

guardband_ul.o: guardband_ul.c common.h
	$(CC) $(CFLAGS) -c $<

common.o: common.c common.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f freq_dl freq_ul
	rm -f inband_dl inband_ul
	rm -f guardband_dl guardband_ul
	rm -f *.o
