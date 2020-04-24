#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  kausf[32] = {
 0x20, 0xDE, 0x4E, 0xB2, 0xB7, 0x5C, 0x8D, 0xAC,
 0x30, 0x1A, 0xD5, 0x27, 0x73, 0xE2, 0xE3, 0x7B,
 0xD7, 0xB4, 0xB0, 0xC2, 0xFD, 0xB1, 0xC3, 0x75,
 0x1B, 0x98, 0xDD, 0x0D, 0xF8, 0x46, 0xAE, 0xD5
};
uint8  snn[32] = {
 /* PLMN-ID: 001.01 */
 '5', 'G',
 ':',
 'm', 'n', 'c', '0', '0', '1',
 '.',
 'm', 'c', 'c', '0', '0', '1',
 '.',
 '3', 'g', 'p', 'p', 'n', 'e', 't', 'w', 'o', 'r', 'k',
 '.',
 'o', 'r', 'g'
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[35];
    uint8  K_SEAF[32];


    memcpy(KEY, kausf, 32);

    /* FC */
    S[0]  = 0x6C;
    /* P0 */
    memcpy(S+1, snn, 32);
    /* L0 */
    S[33] = 0x00;
    S[34] = 0x20;

    mem_dump("Key", KEY, 32);
    mem_dump("S", S, 35);

    kdf(KEY, 32, S, 35, K_SEAF);
    mem_dump("K_SEAF", K_SEAF, 32);


    return 0;
}

