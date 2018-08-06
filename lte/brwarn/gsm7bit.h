#ifndef _GSM_7BIT_H_
#define _GSM_7BIT_H_

#define uint8   unsigned char
#define uint16  unsigned short
#define uint32  unsigned int

#define CEIL_DIV(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define FLOOR_DIV(X, Y)  ((X) / (Y))


uint16 asciiToGsm7bit(uint8 *pIn, uint16 inLen, uint8 *pOut, uint16 outLenMax);
uint16 gsm7bitToAscii(uint8 *pIn, uint16 inLen, uint8 *pOut, uint16 outLenMax);

uint16 sms_encodeGsm7bit(
    uint8  *pOut,
    uint16  outSizeMax,
    uint8  *pIn,
    uint16 *pInSize
);
uint16 sms_decodeGsm7bit(
    uint8  *pIn,
    uint16  inSize,
    uint8  *pOut,
    uint16  outSizeMax
);

void mem_dump(void *addr, int size);


#endif
