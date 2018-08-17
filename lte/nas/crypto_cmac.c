#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include "crypto_cmac.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static const uint8 _rb[CRYPTO_AES_BLOCK_SIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

static void _genSubkeys(tCryptoCmacCtx *pCtx)
{
    AES_KEY aesKey;

    AES_set_encrypt_key(pCtx->key, (CRYPTO_EEA_KEY_SIZE * 8), &aesKey);

    /* L = 0^b */
    uint8 L[CRYPTO_AES_BLOCK_SIZE] = {0};

    /* L = CIPH(0^b, K) */
    AES_encrypt(L, L, &aesKey);

    /* K1 = (L << 1) */
    memcpy(pCtx->k1, L, CRYPTO_AES_BLOCK_SIZE);
    crypto_shiftLeft(pCtx->k1, CRYPTO_AES_BLOCK_SIZE, 1);
    if (L[0] & 0x80)
    {
        /* K1 = (L << 1) xor Rb */
        crypto_xor(pCtx->k1, pCtx->k1, _rb, CRYPTO_AES_BLOCK_SIZE);
    }
    #if (ENABLE_DEBUG)
    LOG_DEBUG("K1 = \n");
    LOG_DUMP(pCtx->k1, CRYPTO_AES_BLOCK_SIZE);
    #endif

    /* K2 = (K1 << 1) */
    memcpy(pCtx->k2, pCtx->k1, CRYPTO_AES_BLOCK_SIZE);
    crypto_shiftLeft(pCtx->k2, CRYPTO_AES_BLOCK_SIZE, 1);
    if (pCtx->k1[0] & 0x80)
    {
        /* K2 = (K1 << 1) xor Rb */
        crypto_xor(pCtx->k2, pCtx->k2, _rb, CRYPTO_AES_BLOCK_SIZE);
    }
    #if (ENABLE_DEBUG)
    LOG_DEBUG("K2 = \n");
    LOG_DUMP(pCtx->k2, CRYPTO_AES_BLOCK_SIZE);
    #endif
}

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
)
{
    int ret = 0;

    /* Check NULL pointer */
    if ((!pCtx) || (!pKey))
    {
        return -1;
    }

    /* Store the key */
    pCtx->keySize = keyLen;
    pCtx->macSize = macLen;
    memcpy(pCtx->key, pKey, keyLen);

    /* Generate subkeys K1 & K2 */
    _genSubkeys(pCtx);

    return ret;
}

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
)
{
    const unsigned int BLOCK_BIT = CRYPTO_AES_BLOCK_SIZE << 3;
    unsigned int i;
    unsigned int blockNum = (inBitLen != 0) ? ((inBitLen + (BLOCK_BIT - 1)) / BLOCK_BIT) : 1;
    unsigned int remBits = (inBitLen != 0) ? (inBitLen % BLOCK_BIT) : BLOCK_BIT;

    uint8 Mn[CRYPTO_AES_BLOCK_SIZE] = {0};
    uint8 C[CRYPTO_AES_BLOCK_SIZE] = {0};

    AES_KEY aesKey;


    if ((!pCtx) || (!pOut) || ((!pIn) && (inBitLen != 0)))
    {
        return;
    }

    #if (ENABLE_DEBUG)
    LOG_DEBUG("n = %u\n", blockNum);
    #endif

    AES_set_encrypt_key(pCtx->key, (CRYPTO_EEA_KEY_SIZE * 8), &aesKey);

    for (i = 0; i < (blockNum - 1); i++)
    {
        #if (ENABLE_DEBUG)
        LOG_DEBUG("M_%u =\n", (i + 1));
        LOG_DUMP(&pIn[i * CRYPTO_AES_BLOCK_SIZE], CRYPTO_AES_BLOCK_SIZE);
        #endif

        crypto_xor(C, C, &pIn[i * CRYPTO_AES_BLOCK_SIZE], CRYPTO_AES_BLOCK_SIZE);

        AES_encrypt(C, C, &aesKey);

        #if (ENABLE_DEBUG)
        LOG_DEBUG("C_%u =\n", (i + 1));
        LOG_DUMP(C, CRYPTO_AES_BLOCK_SIZE);
        #endif
    }

    /* Mn is a complete block */
    if (remBits == 0)
    {
        memcpy(Mn, &pIn[(blockNum - 1) * CRYPTO_AES_BLOCK_SIZE], CRYPTO_AES_BLOCK_SIZE);
        crypto_xor(Mn, Mn, pCtx->k1, CRYPTO_AES_BLOCK_SIZE);
    }
    else
    {
        /* Add padding */
        if (inBitLen != 0)
        {
            unsigned int byteOffset = remBits / 8; 
            unsigned int bitOffset  = remBits % 8;
            memcpy(Mn, &pIn[(blockNum - 1) * CRYPTO_AES_BLOCK_SIZE], byteOffset);
            Mn[byteOffset]  = pIn[(blockNum - 1) * CRYPTO_AES_BLOCK_SIZE + byteOffset] & (0xff << (8 - bitOffset));
            Mn[byteOffset] |= (0x01 << (7 - bitOffset));
        }
        else
        {
            Mn[0] = 0x80;
        }
        crypto_xor(Mn, Mn, pCtx->k2, CRYPTO_AES_BLOCK_SIZE);
    }
    
    #if (ENABLE_DEBUG)
    LOG_DEBUG("M_%u =\n", (i + 1));
    LOG_DUMP(Mn, CRYPTO_AES_BLOCK_SIZE);
    #endif

    crypto_xor(C, C, Mn, CRYPTO_AES_BLOCK_SIZE);

    AES_encrypt(C, C, &aesKey);

    #if (ENABLE_DEBUG)
    LOG_DEBUG("C_%u =\n", (i + 1));
    LOG_DUMP(C, CRYPTO_AES_BLOCK_SIZE);
    #endif

    memcpy(pOut, C, pCtx->macSize);
}

