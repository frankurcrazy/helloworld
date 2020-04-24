#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  K_ASME[32] = {
 0x26, 0x1C, 0xAA, 0x51, 0xBC, 0xF6, 0xD5, 0xD7,
 0xA9, 0x39, 0x16, 0xDB, 0xF6, 0xB6, 0x3D, 0x8B,
 0xBC, 0xD1, 0x88, 0x35, 0x8F, 0x23, 0xCF, 0xB8,
 0x4F, 0x29, 0x15, 0x16, 0xB1, 0x3F, 0x53, 0xE1
};
uint32 NAS_COUNT = 0;

int main(void)
{
    uint8  KEY[32];
    uint8  S[8];
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

