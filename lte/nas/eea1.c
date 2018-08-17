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
 0x5D, 0x5B, 0xFE, 0x75, 0xEB, 0x04, 0xF6, 0x8C,
 0xE0, 0xA1, 0x23, 0x77, 0xEA, 0x00, 0xB3, 0x7D,
 0x47, 0xC6, 0xA0, 0xBA, 0x06, 0x30, 0x91, 0x55,
 0x08, 0x6A, 0x85, 0x9C, 0x43, 0x41, 0xB3, 0x7C
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
                CRYPTO_EEA_ALG1,
                kNasEnc
            );
    if ( error )
    {
        printf("ERR: fail to set the key for EEA1 (%d)\n", error);
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

