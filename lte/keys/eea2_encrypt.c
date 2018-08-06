#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include "kdf.h"


int main(int argc, char *argv[])
{
    AES_KEY  aes_key;
    //----------------------------------------------------------------
    #if (1) /* golden sample 33.401-C.1.1 */
    uint8   P_IN[32] = {
                0x98, 0x1B, 0xA6, 0x82, 0x4C, 0x1B, 0xFB, 0x1A,
                0xB4, 0x85, 0x47, 0x20, 0x29, 0xB7, 0x1D, 0x80,
                0x8C, 0xE3, 0x3E, 0x2C, 0xC3, 0xC0, 0xB5, 0xFC,
                0x1F, 0x3D, 0xE8, 0xA6, 0xDC, 0x66, 0xB1, 0xF0
            };
    uint8   C_OUT[32];
    uint32  P_IN_LEN = 32;

    uint8   K[16] = {
                0xD3, 0xC5, 0xD5, 0x92, 0x32, 0x7F, 0xB1, 0x1C,
                0x40, 0x35, 0xC6, 0x68, 0x0A, 0xF8, 0xC6, 0xD1
            };
    uint32  count = 0x398A59B4;
    uint8   bearer = 0x15;
    uint8   direction = 1;
    #else
    uint8   P_IN[32] = {
                0x98, 0x1B, 0xA6, 0x82, 0x4C, 0x1B, 0xFB, 0x1A,
                0xB4, 0x85, 0x47, 0x20, 0x29, 0xB7, 0x1D, 0x80,
                0x8C, 0xE3, 0x3E, 0x2C, 0xC3, 0xC0, 0xB5, 0xFC,
                0x1F, 0x3D, 0xE8, 0xA6, 0xDC, 0x66, 0xB1, 0xF0
            };
    uint8   C_OUT[32];
    uint32  P_IN_LEN = 32;

    uint8   K[16] = {
                0x09, 0x4F, 0x55, 0xCF, 0xD6, 0xE4, 0x3D, 0xA9,
                0x4E, 0x9E, 0xBB, 0x84, 0x33, 0xCF, 0xB3, 0xED
            };
    uint32  count = 0x398A59B4;
    uint8   bearer = 0x15;
    uint8   direction = 1;
    #endif
    uint8  *IN  = P_IN;
    uint8  *OUT = C_OUT;
    uint8   ECTR_BUF[16];
    uint32  num = 0;
    uint8   CTR[16];
    //----------------------------------------------------------------


    AES_set_encrypt_key(K, 128, &aes_key);
    memset(ECTR_BUF, 0x00, 16);

    memset(CTR, 0x0, 16);
    UINT32_TO_BYTE_ARRAY(count, CTR);
    CTR[4]  = ((bearer    & 0x1F) << 3);
    CTR[4] |= ((direction & 0x01) << 2);
    num = 0;

    // void AES_ctr128_encrypt(
    //          const unsigned char *in,
    //          unsigned char       *out,
    //          const unsigned long  length,
    //          const AES_KEY        *key,
    //          unsigned char         counter[AES_BLOCK_SIZE],
    //          unsigned char         ecount_buf[AES_BLOCK_SIZE],
    //          unsigned int         *num
    //      );
    AES_ctr128_encrypt(
        IN,
        OUT,
        P_IN_LEN,
        &aes_key,
        CTR,
        ECTR_BUF,
        &num
    );

    printf("num = %d\n\n", num);

    mem_dump("Ciphertext", C_OUT, P_IN_LEN);
    //mem_dump("ecount_buf", ECTR_BUF, 16);


    return 0;
}

