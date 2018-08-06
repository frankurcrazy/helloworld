CC = gcc


all: pucch_param rb_mapping

pucch_param: pucch_param.c common.h
	$(CC) -Wall $< -o $@

rb_mapping: rb_mapping.c common.h
	$(CC) -Wall $< -o $@

clean:
	rm -f pucch_param rb_mapping
