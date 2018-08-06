#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


uint8  ck[16] = {
 0xB4, 0x0B, 0xA9, 0xA3, 0xC5, 0x8B, 0x2A, 0x05,
 0xBB, 0xF0, 0xD9, 0x87, 0xB2, 0x1B, 0xF8, 0xCB
};
uint8  ik[16] = {
 0xF7, 0x69, 0xBC, 0xD7, 0x51, 0x04, 0x46, 0x04,
 0x12, 0x76, 0x72, 0x71, 0x1C, 0x6D, 0x34, 0x41
};
uint8  sqnXorAk[6] = {
 /* SQN ^ AK */
 0x55, 0xF3, 0x28, 0xB4, 0x35, 0x77
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
    S[6]  = sqnXorAk[0];
    S[7]  = sqnXorAk[1];
    S[8]  = sqnXorAk[2];
    S[9]  = sqnXorAk[3];
    S[10] = sqnXorAk[4];
    S[11] = sqnXorAk[5];
    S[12] = 0x00;
    S[13] = 0x06;

    mem_dump("Key", KEY, 32);
    mem_dump("S", S, 14);

    kdf(KEY, 32, S, 14, K_ASME);
    mem_dump("K_ASME", K_ASME, 32);


    return 0;
}

