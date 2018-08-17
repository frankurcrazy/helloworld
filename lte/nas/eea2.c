#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_eea.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

uint8  kNasEnc[16] = {
 0xd3, 0xc5, 0xd5, 0x92, 0x32, 0x7f, 0xb1, 0x1c,
 0x40, 0x35, 0xc6, 0x68, 0x0a, 0xf8, 0xc6, 0xd1
};

uint8  msgEncIn[32] = {
 0x98, 0x1b, 0xa6, 0x82, 0x4c, 0x1b, 0xfb, 0x1a,
 0xb4, 0x85, 0x47, 0x20, 0x29, 0xb7, 0x1d, 0x80,
 0x8c, 0xe3, 0x3e, 0x2c, 0xc3, 0xc0, 0xb5, 0xfc,
 0x1f, 0x3d, 0xe8, 0xa6, 0xdc, 0x66, 0xb1, 0xf0
};

uint8  msgEncOut[32] = {0};
uint8  msgEncAns[32] = {
 0xe9, 0xfe, 0xd8, 0xa6, 0x3d, 0x15, 0x53, 0x04,
 0xd7, 0x1d, 0xf2, 0x0b, 0xf3, 0xe8, 0x22, 0x14,
 0xb2, 0x0e, 0xd7, 0xda, 0xd2, 0xf2, 0x33, 0xdc,
 0x3c, 0x22, 0xd7, 0xbd, 0xee, 0xed, 0x8e, 0x78
};

int msgLen = sizeof( msgEncIn );

int dlNasCount = 0x398a59b4;

int bearId = 0x15;

int dir = CRYPTO_DIR_DOWNLINK;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(void)
{
    tCryptoEeaCtx eeaCtx;
    int error;

    memset(&eeaCtx, 0x00, sizeof( tCryptoEeaCtx ));

    error = crypto_eea_setKey(
                &eeaCtx,
                CRYPTO_EEA_ALG2,
                kNasEnc
            );
    if ( error )
    {
        printf("ERR: fail to set the key for EEA2 (%d)\n", error);
        return -1;
    }

    /* cipher the NAS message */
    crypto_eea_encrypt(
        &eeaCtx,
        msgEncOut,
        msgEncIn,
        msgLen,
        bearId,
        dir,
        dlNasCount
    );

    printf("Dump the ciphered data:\n");
    LOG_DUMP(msgEncOut, msgLen);

    printf("Dump the answer:\n");
    LOG_DUMP(msgEncAns, msgLen);

    return 0;
}

