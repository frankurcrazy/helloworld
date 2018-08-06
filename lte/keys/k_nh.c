#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  K_ASME[32] = {
 0x51, 0x9B, 0x55, 0x34, 0xDC, 0x89, 0xFF, 0x8A,
 0x5C, 0x89, 0xE9, 0xA5, 0x5B, 0x65, 0xB5, 0xD7,
 0xFB, 0x93, 0x52, 0x4B, 0x10, 0x5D, 0x66, 0xFE,
 0x24, 0xBF, 0x4B, 0x0C, 0xA7, 0xC3, 0x3E, 0xB9
};
uint32 NAS_COUNT = 0;

int main(void)
{
    uint8  KEY[32];
    uint8  S[32];
    uint8  K_eNB[32];


    memcpy(KEY, K_ASME, 32);

    S[0]  = 0x11;
    S[1]  = ((NAS_COUNT >> 24) & 0xFF);
    S[2]  = ((NAS_COUNT >> 16) & 0xFF);
    S[3]  = ((NAS_COUNT >>  8) & 0xFF);
    S[4]  = ((NAS_COUNT      ) & 0xFF);
    S[5]  = 0x00;
    S[6]  = 0x04;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 7);

    kdf(KEY, 32, S, 7, K_eNB);
    mem_dump("K_eNB", K_eNB, 32);


    return 0;
}

