#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  kseaf[32] = {
 0xD2, 0xBB, 0xC6, 0x3B, 0x0C, 0x14, 0x12, 0x47,
 0x34, 0x1C, 0x0F, 0xCA, 0x3D, 0xC3, 0xBF, 0xFD,
 0xC6, 0xBF, 0xC1, 0x81, 0x34, 0x89, 0x34, 0x2D,
 0xE8, 0x48, 0x7E, 0x7E, 0x4D, 0xC3, 0x72, 0x8B
};
uint8  supi[15] = {
 /* IMSI: 001.01.0123456789 */
 '0', '0', '1',
 '0', '1',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};
uint8  abba[2] = {
 0x00, 0x00
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[22];
    uint8  K_AMF[32];


    memcpy(KEY, kseaf, 32);

    /* FC */
    S[0]  = 0x6D;
    /* P0 */
    memcpy(S+1, supi, 15);
    /* L0 */
    S[16] = 0x00;
    S[17] = 0x0F;
    /* P1 */
    S[18] = abba[0];
    S[19] = abba[1];
    /* L1 */
    S[20] = 0x00;
    S[21] = 0x02;

    mem_dump("Key", KEY, 32);
    mem_dump("S", S, 22);

    kdf(KEY, 32, S, 22, K_AMF);
    mem_dump("K_AMF", K_AMF, 32);


    return 0;
}

