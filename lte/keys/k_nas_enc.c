#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


#define ID_EEA0     0
#define ID_128_EEA1 1
#define ID_128_EEA2 2
#define ID_128_EEA3 3


uint8  K_ASME[32] = {
 0x26, 0x1C, 0xAA, 0x51, 0xBC, 0xF6, 0xD5, 0xD7,
 0xA9, 0x39, 0x16, 0xDB, 0xF6, 0xB6, 0x3D, 0x8B,
 0xBC, 0xD1, 0x88, 0x35, 0x8F, 0x23, 0xCF, 0xB8,
 0x4F, 0x29, 0x15, 0x16, 0xB1, 0x3F, 0x53, 0xE1
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[8];
    uint8  K_NAS_ENC[16];
    uint8  buf[32];


    memcpy(KEY, K_ASME, 32);

    S[0] = 0x15;
    S[1] = 0x01; /* NAS-enc-alg */
    S[2] = 0x00;
    S[3] = 0x01;
    S[4] = ID_128_EEA2; /* EEAx */
    S[5] = 0x00;
    S[6] = 0x01;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 7);

    kdf(KEY, 32, S, 7, buf);
    memcpy(K_NAS_ENC, buf+16, 16);
    mem_dump("K_NAS_ENC", K_NAS_ENC, 16);


    return 0;
}

