#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


#define ID_NIA0     0
#define ID_128_NIA1 1
#define ID_128_NIA2 2
#define ID_128_NIA3 3


uint8  K_gNB[32] = {
 0x61, 0x7C, 0x2F, 0x3D, 0xB7, 0x14, 0xA5, 0xA1,
 0x05, 0x97, 0x59, 0x1A, 0x25, 0xFE, 0xB2, 0x1A,
 0x15, 0x9E, 0xA7, 0x79, 0x61, 0x09, 0x29, 0x2B,
 0x0B, 0x2C, 0xA5, 0x79, 0xD9, 0x00, 0x2B, 0x0C
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[8];
    uint8  K_NAS_INT[16];
    uint8  buf[32];


    memcpy(KEY, K_gNB, 32);

    /* FC */
    S[0] = 0x69;
    /* P0 */
    S[1] = 0x06; /* N-UP-int-alg */
    /* L0 */
    S[2] = 0x00;
    S[3] = 0x01;
    /* P1 */
    S[4] = ID_128_NIA2; /* 5G-IAx */
    /* L1 */
    S[5] = 0x00;
    S[6] = 0x01;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 7);

    kdf(KEY, 32, S, 7, buf);
    memcpy(K_NAS_INT, buf+16, 16);
    mem_dump("K_NAS_INT", K_NAS_INT, 16);


    return 0;
}

