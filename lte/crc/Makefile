CC     = gcc

CRC24A = crc24a
CRC24B = crc24b
CRC16  = crc16
CRC8   = crc8

INC    = types.h


all: $(CRC24A) $(CRC24B) $(CRC16) $(CRC8)


$(CRC24A): $(CRC24A).c $(INC)
	$(CC) -Wall $< -o $@

$(CRC24B): $(CRC24B).c $(INC)
	$(CC) -Wall $< -o $@

$(CRC16): $(CRC16).c $(INC)
	$(CC) -Wall $< -o $@

$(CRC8): $(CRC8).c $(INC)
	$(CC) -Wall $< -o $@

clean:
	rm -f $(CRC24A) $(CRC24B) $(CRC16) $(CRC8)
