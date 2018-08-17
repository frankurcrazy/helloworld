#ifndef _CRYPTO_CMAC_H_
#define _CRYPTO_CMAC_H_

#include "crypto.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Crypto CMAC Context */
typedef struct _tCryptoCmacCtx
{
    uint8 keySize;
    uint8 macSize;
    uint8 key[CRYPTO_AES_MAX_KEY_SIZE];
    uint8 k1[CRYPTO_AES_BLOCK_SIZE];
    uint8 k2[CRYPTO_AES_BLOCK_SIZE];
} tCryptoCmacCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Set a key for a CMAC context.
 * @param [in] pCtx    Pointer to a CMAC context.
 * @param [in] pKey    Pointer to a Key buffer.
 * @param [in] keyLen  Length of the Key buffer (pKey) (Unit: bytes).
 * @param [in] macLen  Length of MAC (< Block Size).
 * @return  Success(0) or failure(-1).
 */
int crypto_cmac_setKey(
    tCryptoCmacCtx *pCtx,
    const uint8    *pKey,
    uint8           keyLen,
    uint8           macLen
);

/**
 * Generate a MAC given a CMAC context.
 * @param [in]  pCtx      Pointer to a CMAC context.
 * @param [out] pOut      Pointer to a output buffer (MAC) (Length is set via crypto_cmac_setKey()).
 * @param [in]  pIn       Pointer to a input buffer (Message).
 * @param [in]  inBitLen  Bit length of message.
 */
void crypto_cmac_mac(
    tCryptoCmacCtx *pCtx,
    uint8          *pOut,
    const uint8    *pIn,
    uint32          inBitLen
);


#endif //_CRYPTO_CMAC_H_
