#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
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


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/* MUL64x.
 * Input V: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling 
 * function.
 * See section 4.3.2 for details.
 */
uint64 MUL64x(uint64 V, uint64 c)
{
    if ( V & 0x8000000000000000ULL )
        return (V << 1) ^ c;
    else
        return V << 1;
}

/* MUL64xPOW.
 * Input V: a 64-bit input.
 * Input i: a positive integer.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling function.
 * See section 4.3.3 for details.
 */
uint64 MUL64xPOW(uint64 V, uint8 i, uint64 c)
{
    if ( i == 0)
        return V; 
    else
        return MUL64x( MUL64xPOW(V,i-1,c) , c);
}

/* MUL64.
 * Input V: a 64-bit input.
 * Input P: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling 
 * function.
 * See section 4.3.4 for details.
 */
uint64 MUL64(uint64 V, uint64 P, uint64 c)
{
    uint64 result = 0;
    int i = 0;

    for ( i=0; i<64; i++)
    {
        if ( ( P>>i ) & 0x1 )
            result ^= MUL64xPOW(V,i,c);
    }

    return result;
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

/* f9.
 * Input key: 128 bit Integrity Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input fresh: 32-bit Random number.
 * Input dir:1 bit, direction of transmission (in the LSB).
 * Input data: length number of bits, input bit stream.
 * Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
 * Output  : 32 bit block used as MAC 
 * Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
 */
void _eia1_mac(
    tCryptoEiaCtx *pCtx,
    uint32         count,
    uint32         fresh,
    uint32         dir,
    uint8         *data,
    uint64         length,
    uint8         *MAC_I
)
{
    uint32 K[4],IV[4], z[5];
    uint32 i=0,D;
    uint32 mac32;
    //static uint8 MAC_I[4] = {0,0,0,0}; /* static memory for the result */
    uint64 EVAL;
    uint64 V;
    uint64 P;
    uint64 Q;
    uint64 c;
    
    uint64 M_D_2;
    int rem_bits = 0;
    
    /* Load the Integrity Key for SNOW3G initialization as in section 4.4. */
    for (i=0; i<4; i++)
        K[3-i] = (pCtx->key[4*i  ] << 24) ^
                 (pCtx->key[4*i+1] << 16) ^
                 (pCtx->key[4*i+2] <<  8) ^
                 (pCtx->key[4*i+3]      );
    
    /* Prepare the Initialization Vector (IV) for SNOW3G initialization as in section 4.4. */
    IV[3] = count;
    IV[2] = fresh;
    IV[1] = count ^ ( dir << 31 ) ;
    IV[0] = fresh ^ (dir << 15);
    
    z[0] = z[1] = z[2] = z[3] = z[4] = 0;
    
    /* Run SNOW 3G to produce 5 keystream words z_1, z_2, z_3, z_4 and z_5. */
    crypto_snow_3g_initialize(&(pCtx->ctx.eia1Ctx), K,IV);
    crypto_snow_3g_generateKeystream(&(pCtx->ctx.eia1Ctx), 5,z);
    
    
    P = (uint64)z[0] << 32 | (uint64)z[1];
    Q = (uint64)z[2] << 32 | (uint64)z[3];
    
    /* Calculation */
    if ((length % 64) == 0)
        D = (length>>6) + 1;
    else
        D = (length>>6) + 2;
    EVAL = 0;
    
    c = 0x1b;
    
    
    /* for 0 <= i <= D-3 */
    for (i=0;i<D-2;i++)
    {
        V = EVAL ^ ((uint64)data[8*i  ]<<56 | (uint64)data[8*i+1]<<48 | (uint64)data[8*i+2]<<40 | (uint64)data[8*i+3]<<32 | 
                    (uint64)data[8*i+4]<<24 | (uint64)data[8*i+5]<<16 | (uint64)data[8*i+6]<< 8 | (uint64)data[8*i+7]);
        EVAL = MUL64(V,P,c);
    }
    
    /* for D-2 */
    rem_bits = length % 64;
    if (rem_bits == 0)
       rem_bits = 64;
    
    M_D_2 = 0;
    i = 0;
    while (rem_bits > 7)
    {
        M_D_2 |= (uint64)data[8*(D-2)+i] << (8*(7-i));
        rem_bits -= 8;
        i++;
    }
    if (rem_bits > 0)
        M_D_2 |= (uint64)(data[8*(D-2)+i] & mask8bit(rem_bits)) << (8*(7-i));
    
    V = EVAL ^ M_D_2;
    EVAL = MUL64(V,P,c);
    
    /* for D-1 */
    EVAL ^= length;  
    
    /* Multiply by Q */
    EVAL = MUL64(EVAL,Q,c);
        
    mac32 = (EVAL >> 32) & 0xffffffff;
    mac32 ^= z[4];
    
    for (i=0; i<4; i++)
        MAC_I[i] = (mac32 >> (8*(3-i))) & 0xff;
}

static void _eia2_mac(
    tCryptoEiaCtx *pCtx,
    uint8         *pMac,
    const uint8   *pMsg,
    uint32         msgBitLen,
    uint8          bearer,
    uint8          dir,
    uint32         count
)
{
    const unsigned int msgLen = (msgBitLen + 7) >> 3;

    unsigned int bits;

    uint8 M[msgLen + 8];

    tCryptoCmacCtx *pCmacCtx = &(pCtx->ctx.eia2Ctx);

    /* Prepare M */
    memset(M, 0, msgLen);
    bits  =  0;
    UINT32_TO_BYTE_ARRAY(count, M);
    bits += 32;
    BYTE_ARRAY_PUT_BITS(M, bits,  5, bearer);
    bits +=  5;
    BYTE_ARRAY_PUT_BITS(M, bits,  1, dir);
    bits +=  1;
    BYTE_ARRAY_PUT_BITS(M, bits, 26, 0);
    bits += 26;
    memcpy((M + 8), pMsg, msgLen);

    #if (ENABLE_DEBUG)
    LOG_DEBUG("Mlen = %u\n", (msgBitLen + 64));
    LOG_DEBUG("M = \n");
    LOG_DUMP(M, msgLen);
    #endif

    crypto_cmac_mac(pCmacCtx, pMac, M, (msgBitLen + 64));
}


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
)
{
    int ret = 0;

    if (!pCtx)
    {
        LOG_ERROR("Null EIA Context\n");
        return -1;
    }

    LOG_INFO("Set a Key and Alg(%d) for EIA\n", alg);
    LOG_DUMP(pKey, CRYPTO_EIA_KEY_SIZE);

    pCtx->alg = alg;

    switch (pCtx->alg)
    {
        case CRYPTO_EIA_ALG0:   /* Null */
            break;

        case CRYPTO_EIA_ALG1:   /* SNOW 3G */
            if (!pKey)
            {
                LOG_ERROR("Key of EIA1 is NULL\n");
                return -1;
            }
            memcpy(pCtx->key, pKey, CRYPTO_EIA_KEY_SIZE);
            break;

        case CRYPTO_EIA_ALG2:   /* AES-128 CMAC */
            if (!pKey)
            {
                LOG_ERROR("Key of EIA2 is NULL\n");
                return -1;
            }
            memcpy(pCtx->key, pKey, CRYPTO_EIA_KEY_SIZE);

            ret = crypto_cmac_setKey(&(pCtx->ctx.eia2Ctx), pKey, CRYPTO_EIA_KEY_SIZE, 4);
            if (ret)
            {
                LOG_ERROR("Set a Key for CMAC Error(%d)\n", ret);
                return ret;
            }
            break;

        default:
            LOG_ERROR("Unknown EIA Algorithm(%d)", pCtx->alg);
            return -1;
    }

    return 0;
}

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
)
{
    if ((!pCtx) || (!pMac) || (!pMsg) ||
        (bearer >= 32) ||
        ((dir != CRYPTO_DIR_UPLINK) && (dir != CRYPTO_DIR_DOWNLINK)))
    {
        LOG_ERROR("Null EIA Context\n");
        return -1;
    }

    switch (pCtx->alg)
    {
        case CRYPTO_EIA_ALG0:   /* Null */
            memset(pMac, 0, CRTPTO_MACI_SIZE);
            break;

        case CRYPTO_EIA_ALG1:   /* SNOW 3G */
            {
                uint32 fresh = ((uint32)bearer) << 27;

                _eia1_mac(pCtx, count, fresh, dir, (uint8*)pMsg, msgBitLen, pMac);
            }
            break;

        case CRYPTO_EIA_ALG2:   /* AES-128 */
            _eia2_mac(pCtx, pMac, pMsg, msgBitLen, bearer, dir, count);
            break;

        default:
            LOG_ERROR("Unknown EIA Algorithm(%d)", pCtx->alg);
            return -1;
    }

    return 0;
}

