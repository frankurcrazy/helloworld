#ifndef _CRYPTO_EEA_H_
#define _CRYPTO_EEA_H_

#include "crypto.h"
#include "crypto_snow_3g.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* EEA Algorithm */
typedef enum
{
    CRYPTO_EEA_ALG0,    /* Null */
    CRYPTO_EEA_ALG1,    /* SNOW 3G - not suppoerted */
    CRYPTO_EEA_ALG2     /* AES-128 */
} tCryptoEeaAlg;

/* EEA Context */
typedef struct _tCryptoEeaCtx
{
    tCryptoEeaAlg alg;

    union
    {
        tCryptoSnow3gCtx    eea1Ctx;
    } ctx;

    uint8 key[CRYPTO_EEA_KEY_SIZE];
} tCryptoEeaCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Set key for EEAx algorithm.
 * @param [in] pCtx  Pointer to a EEA context.
 * @param [in] alg   EEA algorithm.
 * @param [in] pKey  Pointer to a Key buffer (length = CRYPTO_EEA_KEY_SIZE).
 * @return  Success(0) or failure(-1).
 */
int crypto_eea_setKey(
    tCryptoEeaCtx *pCtx, 
    tCryptoEeaAlg  alg, 
    const uint8   *pKey
);

/**
 * EEAx encryption.
 * @param [in]  pCtx    Pointer to a EEA context.
 * @param [out] pOut    Pointer to a output buffer.
 * @param [in]  pIn     Pointer to a input buffer.
 * @param [in]  inLen   Length of the input buffer ('pIn').
 * @param [in]  bearer  a 5-bit Bearer Identity (< 32).
 * @param [in]  dir     a 1-bit Direction (either CRYPTO_DIR_UPLINK or CRYPTO_DIR_DOWNLINK).
 * @param [in]  count   Counter.
 * @return  Success(0) or failure(-1).
 */
int crypto_eea_encrypt(
    tCryptoEeaCtx *pCtx,
    uint8         *pOut,
    const uint8   *pIn,
    uint32         inLen,
    uint8          bearer,
    uint8          dir,
    uint32         count
);

/**
 * EEAx decryption.
 * @param [in]  pCtx    Pointer to a EEA context.
 * @param [out] pOut    Pointer to a output buffer.
 * @param [in]  pIn     Pointer to a input buffer.
 * @param [in]  inLen   Length of the input buffer ('pIn').
 * @param [in]  bearer  a 5-bit Bearer Identity (< 32).
 * @param [in]  dir     a 1-bit Direction (either CRYPTO_DIR_UPLINK or CRYPTO_DIR_DOWNLINK).
 * @param [in]  count   Counter.
 * @return  Success(0) or failure(-1).
*/
int crypto_eea_decrypt(
    tCryptoEeaCtx *pCtx,
    uint8         *pOut,
    const uint8   *pIn,
    uint32         inLen,
    uint8          bearer,
    uint8          dir,
    uint32         count
);


#endif //_CRYPTO_EEA_H_
