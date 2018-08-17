#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
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


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/* f8.
 * Input key: 128 bit Confidentiality Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input bearer: 5-bit Bearer identity (in the LSB side).
 * Input dir:1 bit, direction of transmission.
 * Input data: length number of bits, input bit stream.
 * Input length: 32 bit Length, i.e., the number of bits to be encrypted or 
 *               decrypted.
 * Output data: Output bit stream. Assumes data is suitably memory 
 * allocated.
 * Encrypts/decrypts blocks of data between 1 and 2^32 bits in length as 
 * defined in Section 3.
 */
void _eea1_encrypt(
    tCryptoEeaCtx *pCtx,
    uint32         count,
    uint32         bearer,
    uint32         dir,
    uint8         *data,
    uint32         length
)
{
    uint32 K[4],IV[4];
    int n = ( length + 31 ) / 32;
    int i=0;
    uint32 *KS;
    
    /*Initialisation*/
    
    /* Load the confidentiality key for SNOW 3G initialization as in section 3.4. */
    for (i=0; i<4; i++)
        K[3-i] = (pCtx->key[4*i  ] << 24) ^
                 (pCtx->key[4*i+1] << 16) ^
                 (pCtx->key[4*i+2] <<  8) ^
                 (pCtx->key[4*i+3]      );
    
    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    
    IV[1] = IV[3];
    IV[0] = IV[2];
    
    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS*/
    crypto_snow_3g_initialize(&(pCtx->ctx.eea1Ctx), K,IV);
    
    KS = (uint32 *)malloc(4*n);
    crypto_snow_3g_generateKeystream(&(pCtx->ctx.eea1Ctx), n,(uint32 *)KS); 
    
    /* Exclusive-OR the input data with keystream to generate the output bit stream */
    for (i=0; i<n; i++)
    {
        data[4*i+0] ^= (uint8) (KS[i] >> 24) & 0xff;
        data[4*i+1] ^= (uint8) (KS[i] >> 16) & 0xff;
        data[4*i+2] ^= (uint8) (KS[i] >>  8) & 0xff;
        data[4*i+3] ^= (uint8) (KS[i]      ) & 0xff;
    }
    
    free(KS);
}

static void _eea2_encrypt(
    tCryptoEeaCtx *pCtx,
    uint8         *pOut,
    const uint8   *pIn,
    uint32         inLen,
    uint8          bearer,
    uint8          dir,
    uint32         count
)
{
    uint8 T[CRYPTO_AES_BLOCK_SIZE], O[CRYPTO_AES_BLOCK_SIZE];

    unsigned int bits;
    unsigned int i, blockNum, remBytes;

    AES_KEY aesKey;


    AES_set_encrypt_key(pCtx->key, (CRYPTO_EEA_KEY_SIZE * 8), &aesKey);

    /* Prepare T1 */
    memset(T, 0, CRYPTO_AES_BLOCK_SIZE);
    bits  =  0;
    UINT32_TO_BYTE_ARRAY(count, T);
    bits += 32;
    BYTE_ARRAY_PUT_BITS(T, bits, 5, bearer);
    bits +=  5;
    BYTE_ARRAY_PUT_BITS(T, bits, 1, dir);

    #if (ENABLE_DEBUG)
    LOG_DEBUG("T1 = \n");
    LOG_DUMP(T, CRYPTO_AES_BLOCK_SIZE);
    #endif

    /* Iterative complete blocks */
    blockNum = (inLen + CRYPTO_AES_BLOCK_SIZE - 1) / CRYPTO_AES_BLOCK_SIZE;
    remBytes = (inLen % CRYPTO_AES_BLOCK_SIZE) ? (inLen % CRYPTO_AES_BLOCK_SIZE) : CRYPTO_AES_BLOCK_SIZE;
    #if (ENABLE_DEBUG)
    LOG_DEBUG("Number of blocks          = %u\n", blockNum);
    LOG_DEBUG("Number of remaining bytes = %u\n", remBytes);
    #endif
    for (i = 0; i < (blockNum - 1); i++)
    {
        uint32 temp;

        AES_encrypt(T, O, &aesKey);

        #if (ENABLE_DEBUG)
        LOG_DEBUG("O%u = \n", i+1);
        LOG_DUMP(O, CRYPTO_AES_BLOCK_SIZE);
        #endif

        crypto_xor(O, O, pIn, CRYPTO_AES_BLOCK_SIZE);
        memcpy(pOut, O, CRYPTO_AES_BLOCK_SIZE);

        #if (ENABLE_DEBUG)
        LOG_DEBUG("Out%u = \n", i+1);
        LOG_DUMP(pOut, CRYPTO_AES_BLOCK_SIZE);
        #endif

        pIn  += CRYPTO_AES_BLOCK_SIZE;
        pOut += CRYPTO_AES_BLOCK_SIZE;

        /* Increment T */
        BYTE_ARRAY_TO_UINT32((T + 12), temp);
        temp++;
        UINT32_TO_BYTE_ARRAY(temp, (T + 12));
        if (temp == 0)
        {
            BYTE_ARRAY_TO_UINT32((T + 8), temp);
            temp++;
            UINT32_TO_BYTE_ARRAY(temp, (T + 8));
        }

        #if (ENABLE_DEBUG)
        LOG_DEBUG("T%u = \n", i+2);
        LOG_DUMP(T, CRYPTO_AES_BLOCK_SIZE);
        #endif
    }

    /* The last block */
    AES_encrypt(T, O, &aesKey);

    #if (ENABLE_DEBUG)
    LOG_DEBUG("O%u = \n", i+1);
    LOG_DUMP(O, CRYPTO_AES_BLOCK_SIZE);
    #endif

    crypto_xor(O, O, pIn, remBytes);
    memcpy(pOut, O, remBytes);

    #if (ENABLE_DEBUG)
    LOG_DEBUG("pOut%u = \n", i+1);
    LOG_DUMP(pOut, remBytes);
    #endif
}


/* mask8bit.
 * Input n: an integer in 1-7.
 * Output : an 8 bit mask.
 * Prepares an 8 bit mask with required number of 1 bits on the MSB side.
 */
uint8 mask8bit(int n)
{
  return 0xFF ^ ((1<<(8-n)) - 1);
}

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
)
{
    if (!pCtx)
    {
        LOG_ERROR("Null EEA Context!!\n");
        return -1;
    }

    LOG_INFO("Set a Key and Alg(%d) for EEA\n", alg);
    LOG_DUMP(pKey, CRYPTO_EEA_KEY_SIZE);

    pCtx->alg = alg;

    switch (pCtx->alg)
    {
        case CRYPTO_EEA_ALG0:   /* Null */
            break;

        case CRYPTO_EEA_ALG1:   /* SNOW 3G */
            if (!pKey)
            {
                LOG_ERROR("Key of EEA1 is NULL!!\n");
                return -1;
            }
            memcpy(pCtx->key, pKey, CRYPTO_EEA_KEY_SIZE);
            break;

        case CRYPTO_EEA_ALG2:   /* AES-128 */
            if (!pKey)
            {
                LOG_ERROR("Key of EEA2 is NULL!!\n");
                return -1;
            }
            memcpy(pCtx->key, pKey, CRYPTO_EEA_KEY_SIZE);
            break;

        default:
            LOG_ERROR("Unknown EEA Algorithm(%d)!!", pCtx->alg);
            return -1;
    }

    return 0;
}

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
)
{
    if ((!pCtx) || (!pOut) || (!pIn) ||
        (bearer >= 32) ||
        ((dir != CRYPTO_DIR_UPLINK) && (dir != CRYPTO_DIR_DOWNLINK)))
    {
        LOG_ERROR("Null EEA Context!!\n");
        return -1;
    }

    switch (pCtx->alg)
    {
        case CRYPTO_EEA_ALG0:   /* Null */
            if (pIn != pOut)
            {
                memcpy(pOut, pIn, inLen);
            }
            break;

        case CRYPTO_EEA_ALG1:   /* SNOW 3G */
            if (pIn != pOut)
            {
                uint8 *tmp = malloc(inLen);

                memcpy(tmp, (uint8*)pIn, inLen);
                _eea1_encrypt(pCtx, count, bearer, dir, tmp, (inLen << 3));
                memcpy(pOut, tmp, inLen);
                free(tmp);
            }
            else
            {
                _eea1_encrypt(pCtx, count, bearer, dir, (uint8*)pIn, (inLen << 3));
            }
            break;

        case CRYPTO_EEA_ALG2:   /* AES-128 */
            /* Operations are the same as encrypting */
            _eea2_encrypt(pCtx, pOut, pIn, inLen, bearer, dir, count);
            break;

        default:
            LOG_ERROR("Unknown EEA Algorithm(%d)!!", pCtx->alg);
            return -1;
    }

    return 0;
}

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
)
{
    if ((!pCtx) || (!pOut) || (!pIn) ||
        (bearer >= 32) ||
        ((dir != CRYPTO_DIR_UPLINK) && (dir != CRYPTO_DIR_DOWNLINK)))
    {
        LOG_ERROR("Null EEA Context!!\n");
        return -1;
    }

    switch (pCtx->alg)
    {
        case CRYPTO_EEA_ALG0:   /* Null */
            if (pIn != pOut)
            {
                memcpy(pOut, pIn, inLen);
            }
            break;

        case CRYPTO_EEA_ALG1:   /* SNOW 3G */
            if (pIn != pOut)
            {
                uint8 *tmp = malloc(inLen);

                memcpy(tmp, (uint8*)pIn, inLen);
                _eea1_encrypt(pCtx, count, bearer, dir, tmp, (inLen << 3));
                memcpy(pOut, tmp, inLen);
                free(tmp);
            }
            else
            {
                _eea1_encrypt(pCtx, count, bearer, dir, (uint8*)pIn, (inLen << 3));
            }
            break;

        case CRYPTO_EEA_ALG2:   /* AES-128 */
            /* Operations are the same as encrypting */
            _eea2_encrypt(pCtx, pOut, pIn, inLen, bearer, dir, count);
            break;

        default:
            LOG_ERROR("Unknown EEA Algorithm(%d)!!", pCtx->alg);
            return -1;
    }

    return 0;
}


