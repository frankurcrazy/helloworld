#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "kdf.h"


uint8  _rand[16] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8  xres_star[16] = {
 0x15, 0x72, 0x48, 0xEF, 0x1B, 0x85, 0x2F, 0x19,
 0xC3, 0xE6, 0xA5, 0xE5, 0xBE, 0xDB, 0x67, 0x5C
};

int main(void)
{
    uint8  S[32];
    uint8  HXRES_STAR[16];
    uint8  buf[32];


    /* P0 */
    memcpy(S, _rand, 16);
    /* P1 */
    memcpy(S+16, xres_star, 16);

    mem_dump("S", S, 32);

    SHA256(S, 32, buf);
    memcpy(HXRES_STAR, buf+16, 16);
    mem_dump("HXRES*", HXRES_STAR, 16);


    return 0;
}

