CC = gcc
CFLAGS = -Wall

all: CCE_REG_mapping CCE_index

CCE_REG_mapping: CCE_REG_mapping.c
	$(CC) $(CFLAGS) $< -o $@

CCE_index: CCE_index.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f CCE_REG_mapping CCE_index
