#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  K_AMF[32] = {
 0x7E, 0x9F, 0x97, 0x70, 0x7C, 0xEF, 0x48, 0x31,
 0x8D, 0xF1, 0x82, 0xA7, 0xF9, 0xE7, 0xBA, 0x83,
 0x8A, 0x6F, 0xB3, 0x6A, 0xB8, 0x3A, 0x7D, 0x1B,
 0xAF, 0x26, 0x10, 0xB9, 0x7A, 0x5B, 0x9D, 0xCF
};
uint32 NAS_COUNT = 0;

int main(void)
{
    uint8  KEY[32];
    uint8  S[10];
    uint8  K_gNB[32];


    memcpy(KEY, K_AMF, 32);

    /* FC */
    S[0] = 0x6E;
    /* P0 */
    S[1]  = ((NAS_COUNT >> 24) & 0xFF);
    S[2]  = ((NAS_COUNT >> 16) & 0xFF);
    S[3]  = ((NAS_COUNT >>  8) & 0xFF);
    S[4]  = ((NAS_COUNT      ) & 0xFF);
    /* L0 */
    S[5] = 0x00;
    S[6] = 0x04;
    /* P1 */
    S[7] = 0x01; /* 3GPP access */
    /* L1 */
    S[8] = 0x00;
    S[9] = 0x01;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 10);

    kdf(KEY, 32, S, 10, K_gNB);
    mem_dump("K_gNB", K_gNB, 32);


    return 0;
}

