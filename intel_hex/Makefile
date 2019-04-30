CC      = gcc
CFLAGS  = -Wall -c
LDFLAGS = -Wall


INTEL_HEX = intel_hex

all: $(INTEL_HEX)
	strip $(INTEL_HEX)

$(INTEL_HEX): $(INTEL_HEX).o
	$(CC) $(LDFLAGS) $< -o $@

$(INTEL_HEX).o: $(INTEL_HEX).c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(INTEL_HEX) *.o

