#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_eia.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

uint8  kNasInt[16] = {
 0x2b, 0xd6, 0x45, 0x9f, 0x82, 0xc5, 0xb3, 0x00,
 0x95, 0x2c, 0x49, 0x10, 0x48, 0x81, 0xff, 0x48
};

uint8  msgIn[8] = {
 0x33, 0x32, 0x34, 0x62, 0x63, 0x39, 0x38, 0x40
};

uint8  cmacOut[4] = {0};
uint8  cmacAns[4] = {
 0xd2, 0xc8, 0x28, 0x7d
};

int msgLen = sizeof( msgIn );
int msgLenInBits = 58;

int ulNasCount = 0x38a6f056;

int bearId = 0x18;

int dir = CRYPTO_DIR_UPLINK;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(void)
{
    tCryptoEiaCtx eiaCtx;
    int error;

    memset(&eiaCtx, 0x00, sizeof( tCryptoEiaCtx ));

    error = crypto_eia_setKey(
                &eiaCtx,
                CRYPTO_EIA_ALG1,
                kNasInt
            );
    if ( error )
    {
        printf("ERR: fail to set the key for EIA1 (%d)\n", error);
        return -1;
    }

    /* calculate message authentication code */
    crypto_eia_mac(
        &eiaCtx,
        cmacOut,
        msgIn,
        msgLenInBits,
        bearId,
        dir,
        ulNasCount
    );

    printf("NAS MAC calc: len[%d] bits, bearer ID[%d], DIR[%d], NAS count[%d]\n",
        msgLenInBits,
        bearId,
        dir,
        ulNasCount
    );
    LOG_DUMP(cmacOut, 4);

    printf("Dump the answer:\n");
    LOG_DUMP(cmacAns, 4);

    return 0;
}

