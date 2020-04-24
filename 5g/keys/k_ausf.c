#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  ck[16] = {
 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00
};
uint8  ik[16] = {
 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01
};
uint8  ak[6] = {
 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};
uint8  sqn[6] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
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
    uint8  S[43];
    uint8  K_AUSF[32];


    memcpy(KEY,    ck, 16);
    memcpy(KEY+16, ik, 16);

    /* FC */
    S[0]  = 0x6A;
    /* P0 */
    memcpy(S+1, snn, 32);
    /* L0 */
    S[33] = 0x00;
    S[34] = 0x20;
    /* P1 */
    S[35] = (sqn[0] ^ ak[0]);
    S[36] = (sqn[1] ^ ak[1]);
    S[37] = (sqn[2] ^ ak[2]);
    S[38] = (sqn[3] ^ ak[3]);
    S[39] = (sqn[4] ^ ak[4]);
    S[40] = (sqn[5] ^ ak[5]);
    /* L1 */
    S[41] = 0x00;
    S[42] = 0x06;

    mem_dump("Key", KEY, 32);
    mem_dump("S", S, 43);

    kdf(KEY, 32, S, 43, K_AUSF);
    mem_dump("K_AUSF", K_AUSF, 32);


    return 0;
}

