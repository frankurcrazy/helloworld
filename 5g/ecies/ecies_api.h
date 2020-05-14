#ifndef __ECIES_API_H__
#define __ECIES_API_H__

#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define EC_DOMAIN_CURVE25519 (1034)  /* ==> NID_X25519 */
#define EC_DOMAIN_SECP256R1  (415)   /* ==> NID_X9_62_prime256v1 */


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    ECIES_NW_KEY_ASSIGNED = 0,
    ECIES_NW_KEY_RANDOM = 1,
    ECIES_NW_KEY_DER = 2,
    ECIES_NW_KEY_END
} tEciesNwKeyWay;

typedef enum
{
    ECIES_PROFILE_A = 0,
    ECIES_PROFILE_B = 1,
    ECIES_PROFILE_END
} tEciesProfile;


typedef struct _tEciesKeyInfo
{
    int     profile;  /* ==> tEciesProfile */

    uint8  *pNwPubKey;
    uint32  nwPubkeyLen;

    uint8  *pNwPrivKey;
    uint32  nwPrivkeyLen;
} tEciesKeyInfo;

typedef struct _tEciesEncrypInfo
{
    int     profile;  /* ==> tEciesProfile */

    uint8  *pPubKey;
    uint32  pubkeyLen;

    uint8  *pCipherMsg;
    uint32  cipherMsgLen;

    uint8  *pMacTag;
    uint8   macTagLen;
} tEciesEncrypInfo;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * ECIES set key mode.
 * @param [in]  type  A @ref tEciesNwKeyWay enumeration.
 * @param [in]  curve  EC domain parameter.
 * @param [in]  pFilename  Private key in .der file.
 * @param [in]  pPrivKeyStr  Private key in HEX string format.
 * @returns  A @ref tEciesKeyInfo object.
 */
tEciesKeyInfo *ecies_setKeyMode(
    int   type,
    int   curve,     
    char *pFilename,
    char *pPrivKeyStr
);

/**
 * ECIES message encryption.
 * @param [in]  profile  A @ref tEciesProfile enumeration.
 * @param [in]  pNwPubKey  Network public key.
 * @param [in]  nwPubKeyLen  Network public key length.
 * @param [in]  pPlainMsg  Plaintext message.
 * @param [in]  plainMsgLen  Plaintext message length.
 * @returns  A @ref tEciesEncrypInfo object.
 */
tEciesEncrypInfo *ecies_encodeMsg(
    int     profile,
    uint8  *pNwPubKey,
    uint32  nwPubKeyLen,
    uint8  *pPlainMsg,
    uint32  plainMsgLen
);

/**
 * ECIES message decryption.
 * @param [in]  pEncrypInfo  A @ref tEciesEncrypInfo object.
 * @param [in]  pNwPrivKey  Network private key.
 * @param [in]  nwPrivKeyLen  Network private key length.
 * @param [in]  ppPlainMsg  Plaintext message.
 * @returns  Plaintext message length.
 */
uint32 ecies_decodeMsg(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPrivKey,
    uint32             nwPrivKeyLen,
    uint8            **ppPlainMsg  
);

/**
 * ECIES free encryption object.
 * @param [in]  pObj  A @ref tEciesEncrypInfo object.
 */
void ecies_freeEncrypInfo(tEciesEncrypInfo *pObj);

/**
 * ECIES free key object.
 * @param [in]  pObj  A @ref tEciesKeyInfo object.
 */
void ecies_freeKeyInfo(tEciesKeyInfo *pObj);


#endif
