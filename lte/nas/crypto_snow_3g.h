#ifndef _CRYPTO_SNOW_3G_H_
#define _CRYPTO_SNOW_3G_H_

#include "crypto.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Structure of SNOW 3G Context */
typedef struct _tCryptoSnow3gCtx {
    /* LFSR */
    uint32 LFSR_S0;
    uint32 LFSR_S1;
    uint32 LFSR_S2;
    uint32 LFSR_S3;
    uint32 LFSR_S4;
    uint32 LFSR_S5;
    uint32 LFSR_S6;
    uint32 LFSR_S7;
    uint32 LFSR_S8;
    uint32 LFSR_S9;
    uint32 LFSR_S10;
    uint32 LFSR_S11;
    uint32 LFSR_S12;
    uint32 LFSR_S13;
    uint32 LFSR_S14;
    uint32 LFSR_S15;
    
    /* FSM */
    uint32 FSM_R1;
    uint32 FSM_R2;
    uint32 FSM_R3;
} tCryptoSnow3gCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/* Initialization.
 * Input k[4]: Four 32-bit words making up 128-bit key.
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 * Output: All the LFSRs and FSM are initialized for key generation.
 * See Section 4.1.
 */
void crypto_snow_3g_initialize(tCryptoSnow3gCtx *pCtx, uint32 k[4], uint32 IV[4]);

/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes 
 *          memory is allocated already.
 * output: generated keystream which is filled in z 
 * See section 4.2.
 */
void crypto_snow_3g_generateKeystream(tCryptoSnow3gCtx *pCtx, uint32 n, uint32 *ks);


#endif //_CRYPTO_SNOW_3G_H_
