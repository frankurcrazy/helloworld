CC = gcc

all: epdcch prb-pair ecce ereg dmrs nEPDCCH

epdcch: epdcch.c
	$(CC) -Wall $< -o $@

prb-pair: prb-pair.c
	$(CC) -Wall $< -o $@

ecce: ecce.c
	$(CC) -Wall $< -o $@

ereg: ereg.c
	$(CC) -Wall $< -o $@

dmrs: dmrs.c
	$(CC) -Wall $< -o $@

nEPDCCH: nEPDCCH.c
	$(CC) -Wall $< -o $@

clean:
	rm -f epdcch prb-pair ecce ereg dmrs nEPDCCH
