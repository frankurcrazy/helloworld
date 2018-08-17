#ifndef _CRYPTO_EIA_H_
#define _CRYPTO_EIA_H_

#include "crypto.h"
#include "crypto_snow_3g.h"
#include "crypto_cmac.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* EIA Algorithm */
typedef enum
{
    CRYPTO_EIA_ALG0,    /* Null */
    CRYPTO_EIA_ALG1,    /* SNOW 3G - not suppoerted */
    CRYPTO_EIA_ALG2     /* AES-128 CMAC */
} tCryptoEiaAlg;

/* EIA Context */
typedef struct _tCryptoEiaCtx
{
    tCryptoEiaAlg alg;

    union
    {
        tCryptoSnow3gCtx    eia1Ctx;

        tCryptoCmacCtx      eia2Ctx; /* AES-128 CMAC */

    } ctx;

    uint8 key[CRYPTO_EIA_KEY_SIZE];
} tCryptoEiaCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Set key for EIAx algorithm.
 * @param [in] pCtx  Pointer to a EIA context.
 * @param [in] alg   EIAx algorithm.
 * @param [in] pKey  Pointer to a Key buffer (length = CRYPTO_EIA_KEY_SIZE).
 * @returns  Success(0) or failure(-1).
 */
int crypto_eia_setKey(
    tCryptoEiaCtx *pCtx,
    tCryptoEiaAlg  alg,
    const uint8   *pKey
);

/**
 * EIA message authentication code.
 * @param [in]  pCtx       Pointer to a EEA context.
 * @param [out] pMac       Pointer to a MAC (4 bytes).
 * @param [in]  pMsg       Pointer to a message buffer.
 * @param [in]  msgBitLen  Bit length of the message buffer ('pMsg').
 * @param [in]  bearer     a 5-bit Bearer Identity (< 32).
 * @param [in]  dir        a 1-bit Direction (either CRYPTO_DIR_UPLINK or CRYPTO_DIR_DOWNLINK).
 * @param [in]  count      Counter.
 * @returns  Success(0) or failure(-1).
 */
int crypto_eia_mac(
    tCryptoEiaCtx *pCtx,
    uint8         *pMac,
    const uint8   *pMsg,
    uint32         msgBitLen,
    uint8          bearer,
    uint8          dir,
    uint32         count
);


#endif //_CRYPTO_EIA_H_
