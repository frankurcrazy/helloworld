CC = gcc

all: cce res_alloc_riv comb_r

cce: cce.c
	$(CC) -Wall $< -o $@

res_alloc_riv: res_alloc_riv.c
	$(CC) -Wall $< -o $@

comb_r: comb_r.c
	$(CC) -Wall $< -o $@

clean:
	rm -f cce res_alloc_riv comb_r
