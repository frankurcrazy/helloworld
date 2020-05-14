#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include "ecies_api.h"
#include "ecies_core.h"


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
)
{
    tEciesKeyInfo *pKeyInfo = NULL;
    const EC_GROUP *pEcGroup = NULL;
    EC_KEY   *pEcKey    = NULL;
    uint8    *pPubKey   = NULL;
    uint8    *pPrivKey  = NULL;
    uint32    pubKeyLen = 0;
    uint32    privKeyLen= 0;
    EVP_PKEY *pEvKey = NULL;
    void *pKey = NULL;


    switch ( type )
    {
        case ECIES_NW_KEY_ASSIGNED:
            pKey = ecies_setNwKeyByAssign(curve, pPrivKeyStr);
            break;
        case ECIES_NW_KEY_RANDOM:
            pKey = ecies_setNwKeyViaRandom( curve );
            break;
        case ECIES_NW_KEY_DER:
           /*
            * Create a BIO object wrapping a file pointer to read the EC key file
            * in DER format. Then read in and parse the EC key from the file.
            */
            pKey = ecies_setNwKeyByDer(curve, pFilename);
            break;
        default:
            EC_WARN("unknow type %d\n", type);
    }

    if (NID_X25519 == curve)
    {
        pEvKey = (EVP_PKEY *)pKey;
        if ( pEvKey )
        {
            pKeyInfo = malloc( sizeof( tEciesKeyInfo ) );
            if ( pKeyInfo )
            {
                /* profile A: NID_X25519 */
                pKeyInfo->profile = ECIES_PROFILE_A;
                pKeyInfo->nwPubkeyLen  = ECIES_X25519_KEYLEN;
                pKeyInfo->nwPrivkeyLen = ECIES_X25519_KEYLEN;
                pKeyInfo->pNwPubKey    = malloc(ECIES_X25519_KEYLEN);
                pKeyInfo->pNwPrivKey   = malloc(ECIES_X25519_KEYLEN);
                
                EVP_PKEY_get_raw_public_key(
                    pEvKey,
                    pKeyInfo->pNwPubKey,
                    &(pKeyInfo->nwPubkeyLen)
                );
                
                EVP_PKEY_get_raw_private_key(
                    pEvKey,
                    pKeyInfo->pNwPrivKey,
                    &(pKeyInfo->nwPrivkeyLen)
                );
            }
            EVP_PKEY_free(pEvKey);
        }
    }
    else if (NID_X9_62_prime256v1 == curve)
    {
        pEcKey = (EC_KEY *)pKey;
        if ( pEcKey )
        {
            /* Get the EC key's public key in a binary array format. */
            ecies_publicKeyToBin(pEcKey, &pPubKey, &pubKeyLen);

            /* Get the EC key's private key in a binary array format. */
            ecies_privateKeyToBin(pEcKey, &pPrivKey, &privKeyLen);
            
            /* Get the curve parameters from the EC key. */
            pEcGroup = EC_KEY_get0_group(pEcKey);

            pKeyInfo = malloc( sizeof( tEciesKeyInfo ) );
            if ( pKeyInfo )
            {
                /* profile B: NID_X9_62_prime256v1 */
                pKeyInfo->profile = ECIES_PROFILE_B;

                if (pubKeyLen && privKeyLen)
                {
                   pKeyInfo->nwPubkeyLen = pubKeyLen;
                   pKeyInfo->pNwPubKey   = malloc(pubKeyLen);
                   memcpy(pKeyInfo->pNwPubKey, pPubKey, pubKeyLen);
                   
                   pKeyInfo->nwPrivkeyLen = privKeyLen;
                   pKeyInfo->pNwPrivKey   = malloc(privKeyLen);
                   memcpy(pKeyInfo->pNwPrivKey, pPrivKey, privKeyLen);
                }
            }
            free( pPubKey );
            free( pPrivKey );
            EC_KEY_free( pEcKey );
        }
    }
    else
    {
        EC_WARN("%s: unknow curve %d\n", __func__, curve);
    }

    EC_DUMP(
       "Home Network Private Key",
        pKeyInfo->pNwPrivKey,
        pKeyInfo->nwPrivkeyLen
    );
    EC_DUMP(
        "Home Network Public Key",
        pKeyInfo->pNwPubKey,
        pKeyInfo->nwPubkeyLen
    );

    return pKeyInfo;
}


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
)
{
    tEciesEncrypInfo *pEncrypInfo;
    uint8  *pSkey = NULL;
    uint32  skeyLen;
    uint32  keLen  = ECIES_PARAM_ENCKEYLEN;
    uint32  icbLen = ECIES_PARAM_ICBLEN;
    uint32  kmLen  = ECIES_PARAM_MACKEYLEN;
    uint8   dataKey[64];


    pEncrypInfo = malloc( sizeof( tEciesEncrypInfo ) );
    memset(pEncrypInfo, 0, sizeof( tEciesEncrypInfo ) );

    pEncrypInfo->profile = profile;

    if (ECIES_PROFILE_A == pEncrypInfo->profile)
    {
        skeyLen = ecise_shareKeyByX25519ByNwPubKey(
                      pEncrypInfo,
                      pNwPubKey,
                      nwPubKeyLen,
                      &pSkey
                  );
    }
    else if (ECIES_PROFILE_B == pEncrypInfo->profile)
    {
        skeyLen = ecies_shareKeyBySecp256r1ByNwPubKey(
                      pEncrypInfo,
                      pNwPubKey,
                      nwPubKeyLen,
                      &pSkey
                  );
    }
    else
    {
        EC_WARN("%s: unknow profile %d\n", __func__, pEncrypInfo->profile);
    }

    EC_DUMP("Eph. Public Key", pEncrypInfo->pPubKey, pEncrypInfo->pubkeyLen);
    EC_DUMP("Eph. Shared Key", pSkey, skeyLen);

    ECDH_KDF_X9_62(
        dataKey,
        (keLen + icbLen + kmLen),
        pSkey,
        skeyLen,
        pEncrypInfo->pPubKey,
        pEncrypInfo->pubkeyLen,
        EVP_sha256()
    );
    EC_DUMP("Eph. Enc. Key", dataKey, keLen);
    EC_DUMP("ICB", (dataKey + keLen), icbLen);
    EC_DUMP("Plaintext block", pPlainMsg , plainMsgLen);

    /* Encrypt the data using 128 AES-CTR. */
    ecies_aesCtr128AndSha256MacEncrypt( 
        pEncrypInfo,
        dataKey,
        keLen,
        icbLen,
        kmLen ,
        pPlainMsg ,
        plainMsgLen 
    );

    EC_DUMP("Cipher-text value", pEncrypInfo->pCipherMsg, pEncrypInfo->cipherMsgLen);
    EC_DUMP("Eph. mac key", (dataKey + keLen + icbLen), kmLen);
    EC_DUMP("MAC-tag value", pEncrypInfo->pMacTag, pEncrypInfo->macTagLen);
    
    free( pSkey );

    return pEncrypInfo;
}


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
)
{
    uint8  *pSkey = NULL;
    uint32  skeyLen;
    uint32  plainMsgLen = 0;
    uint32  keLen  = ECIES_PARAM_ENCKEYLEN;
    uint32  icbLen = ECIES_PARAM_ICBLEN;
    uint32  kmLen  = ECIES_PARAM_MACKEYLEN;
    uint8   dataKey[64];

    if (ECIES_PROFILE_A == pEncrypInfo->profile)
    {
        skeyLen = ecies_shareKeyByX25519ByNwPrivKey(
                      pEncrypInfo,
                      pNwPrivKey,
                      nwPrivKeyLen,
                      &pSkey
                  );       
    }
    else if (ECIES_PROFILE_B == pEncrypInfo->profile)
    {
        skeyLen = ecies_shareKeyBySecp256r1ByNwPrivKey(
                      pEncrypInfo,
                      pNwPrivKey,
                      nwPrivKeyLen,
                      &pSkey
                  );
    }
    else
    {
        EC_WARN("%s: unknow profile %d\n", __func__, pEncrypInfo->profile);
    }

    EC_DUMP("Eph. Public Key", pEncrypInfo->pPubKey, pEncrypInfo->pubkeyLen);
    EC_DUMP("Eph. Shared Key", pSkey, skeyLen);

    ECDH_KDF_X9_62(
        dataKey,
        keLen+icbLen+kmLen,
        pSkey,
        skeyLen,
        pEncrypInfo->pPubKey,
        pEncrypInfo->pubkeyLen,
        EVP_sha256()
    );
    EC_DUMP("Eph. Enc. Key", dataKey, keLen);
    EC_DUMP("ICB", (dataKey + keLen), icbLen);
    EC_DUMP("Eph.mac key", (dataKey + keLen + icbLen), kmLen);

    plainMsgLen = ecies_aesCtr128AndSha256MacDecrypt(
                      pEncrypInfo,
                      dataKey,
                      keLen,
                      icbLen,
                      kmLen,
                      ppPlainMsg
                  );
    if (plainMsgLen != 0)
    {
        EC_DUMP("Plaintext block", *ppPlainMsg, plainMsgLen);
    }

    free( pSkey );

    return plainMsgLen;
}


/**
 * ECIES free encryption object.
 * @param [in]  pObj  A @ref tEciesEncrypInfo object.
 */
void ecies_freeEncrypInfo(tEciesEncrypInfo *pObj)
{
    if ( pObj )
    {
        if ( pObj->pPubKey )
        {
           free( pObj->pPubKey ); 
        }

        if ( pObj->pMacTag )
        {
            free( pObj->pMacTag );
        }

        if ( pObj->pCipherMsg )
        {
            free( pObj->pCipherMsg );
        }

        free( pObj );
    }
}


/**
 * ECIES free key object.
 * @param [in]  pObj  A @ref tEciesKeyInfo object.
 */
void ecies_freeKeyInfo(tEciesKeyInfo *pObj)
{
    if ( pObj )
    {
        if ( pObj->pNwPubKey )
        {
           free( pObj->pNwPubKey );
        }

        if ( pObj->pNwPrivKey )
        {
            free( pObj->pNwPrivKey );
        }

        free( pObj );
    }
}

