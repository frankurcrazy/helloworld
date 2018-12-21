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
uint8  snId[3] = {
 /* PLMN-ID: 001.01 */
 0x00, 0xF1, 0x10
};

int main(void)
{
    uint8  KEY[32];
    uint8  S[32];
    uint8  K_ASME[32];


    memcpy(KEY,    ck, 16);
    memcpy(KEY+16, ik, 16);

    S[0]  = 0x10;
    S[1]  = snId[0];
    S[2]  = snId[1];
    S[3]  = snId[2];
    S[4]  = 0x00;
    S[5]  = 0x03;
    S[6]  = (sqn[0] ^ ak[0]);
    S[7]  = (sqn[1] ^ ak[1]);
    S[8]  = (sqn[2] ^ ak[2]);
    S[9]  = (sqn[3] ^ ak[3]);
    S[10] = (sqn[4] ^ ak[4]);
    S[11] = (sqn[5] ^ ak[5]);
    S[12] = 0x00;
    S[13] = 0x06;

    mem_dump("Key", KEY, 32);
    mem_dump("S", S, 14);

    kdf(KEY, 32, S, 14, K_ASME);
    mem_dump("K_ASME", K_ASME, 32);


    return 0;
}

