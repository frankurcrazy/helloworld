#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


#define ID_NIA0     0
#define ID_128_NIA1 1
#define ID_128_NIA2 2
#define ID_128_NIA3 3


uint8  K_AMF[32] = {
 0x7E, 0x9F, 0x97, 0x70, 0x7C, 0xEF, 0x48, 0x31,
 0x8D, 0xF1, 0x82, 0xA7, 0xF9, 0xE7, 0xBA, 0x83,
 0x8A, 0x6F, 0xB3, 0x6A, 0xB8, 0x3A, 0x7D, 0x1B,
 0xAF, 0x26, 0x10, 0xB9, 0x7A, 0x5B, 0x9D, 0xCF
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[8];
    uint8  K_NAS_INT[16];
    uint8  buf[32];


    memcpy(KEY, K_AMF, 32);

    /* FC */
    S[0] = 0x69;
    /* P0 */
    S[1] = 0x02; /* N-NAS-int-alg */
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

