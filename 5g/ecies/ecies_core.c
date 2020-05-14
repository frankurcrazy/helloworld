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

#define PROFILE_B_COMPRESSED (1)
#if PROFILE_B_COMPRESSED
# define SUCI_CONVERSION POINT_CONVERSION_COMPRESSED
#else
# define SUCI_CONVERSION POINT_CONVERSION_UNCOMPRESSED
#endif

#define ECIES_USE_SPEC_PARAMETER (1)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void *ecies_setNwKeyByAssign(int curve, char *pPrivKeyStr)
{
    void     *pOutKey = NULL;
    EC_KEY   *pEcKey = NULL;
    EVP_PKEY *pEvkey = NULL;
    BIGNUM   *pPrv = NULL; 
    EC_POINT *pPub = NULL;
    EC_GROUP *pEcGroup = NULL;
    uint8    *pPrivKey = NULL;
    uint32    privKeyLen;

    if (NID_X25519 == curve)
    {
        BN_hex2bn(&pPrv, pPrivKeyStr);
        privKeyLen = BN_num_bytes( pPrv );
        pPrivKey = OPENSSL_malloc( privKeyLen );
        BN_bn2bin(pPrv, pPrivKey);

        /* calculate the public key */ 
        pEvkey = EVP_PKEY_new_raw_private_key(
                     EVP_PKEY_X25519,
                     NULL,
                     pPrivKey,
                     privKeyLen
                 );

        if ( pPrv )
        {
            BN_clear_free( pPrv );
        }

        if ( pPrivKey )
        {
            free( pPrivKey );
        }

        pOutKey = pEvkey;
    }
    else if (NID_X9_62_prime256v1 == curve)
    {
        pEcKey = EC_KEY_new();
        pEcGroup = EC_GROUP_new_by_curve_name( curve );
        EC_KEY_set_group(pEcKey, pEcGroup);
        BN_hex2bn(&pPrv, pPrivKeyStr);
        pPub = EC_POINT_new( pEcGroup );

        /* calculate the public key */ 
        EC_POINT_mul(pEcGroup, pPub, pPrv, NULL, NULL, NULL);

        /* add the private & public key to the EC_KEY structure */
        EC_KEY_set_private_key(pEcKey, pPrv);
        EC_KEY_set_public_key(pEcKey, pPub);

        pOutKey = pEcKey;

        if ( pPrv )
        {
            BN_clear_free( pPrv );
        }

        if ( pPub )
        {
            OPENSSL_free( pPub );
        }
    }
    else
    {
        EC_WARN("%s: unknow curve %d\n", __func__, curve);
    }
    
    return pOutKey;
}

void *ecies_setNwKeyViaRandom(int curve)
{
    void     *pOutKey = NULL;
    EC_KEY   *pEcKey  = NULL;
    EVP_PKEY *pEvkey  = NULL;
    EVP_PKEY_CTX *pCtx = NULL;

    if (NID_X25519 == curve)
    {
        pCtx = EVP_PKEY_CTX_new_id(curve, NULL);
        EVP_PKEY_keygen_init( pCtx );
        EVP_PKEY_keygen(pCtx, &pEvkey);
        EVP_PKEY_CTX_free( pCtx );
        pOutKey = pEvkey;
    }
    else if (NID_X9_62_prime256v1 == curve)
    {
        pEcKey = EC_KEY_new_by_curve_name(curve);
        EC_KEY_generate_key( pEcKey );
        pOutKey = pEcKey;
    }
    else
    {
        EC_WARN("%s: unknow curve %d\n", __func__, curve);
    }

    return pOutKey;
}

void *ecies_setNwKeyByDer(int curve, char *pFilename)
{
    void     *pOutKey = NULL;
    EC_KEY   *pEcKey  = NULL;
    EVP_PKEY *pEvKey  = NULL;
    BIO      *pBioKey = BIO_new_file(pFilename, "r");

    if (NID_X25519 == curve)
    {
        if ( pBioKey )
        {
            pEvKey = d2i_PrivateKey_bio(pBioKey, NULL);
            pOutKey = pEvKey;
        }
    }
    else if (NID_X9_62_prime256v1 == curve)
    {
        if ( pBioKey )
        {
            pEcKey = d2i_ECPrivateKey_bio(pBioKey, NULL);
            pOutKey = pEcKey;
        }
    }
    else
    {
        EC_WARN("%s: unknow curve %d\n", __func__, curve);
    }

    if ( pBioKey )
    {
        BIO_free( pBioKey );
    }

    return pOutKey;
}


/* Convert an EC key's public key to a binary array. */
int ecies_publicKeyToBin(
    EC_KEY  *pEcKey,
    uint8  **ppPubkey,
    uint32  *pPubkLen
)
{
    const EC_GROUP *pEcGroup  = EC_KEY_get0_group(pEcKey);
    const EC_POINT *pPub      = EC_KEY_get0_public_key(pEcKey);
    BIGNUM         *pPubBn    = BN_new();
    BN_CTX         *pPubBnCtx = BN_CTX_new();

    BN_CTX_start( pPubBnCtx );

    EC_POINT_point2bn(
        pEcGroup,
        pPub,
        SUCI_CONVERSION,
        pPubBn,
        pPubBnCtx
    );

    *pPubkLen = BN_num_bytes( pPubBn );
    *ppPubkey = OPENSSL_malloc( *pPubkLen );

    if (BN_bn2bin(pPubBn, *ppPubkey) != *pPubkLen)
    {
        EC_ERROR("Failed to decode pubkey\n");
    }

    BN_CTX_end( pPubBnCtx );
    BN_CTX_free( pPubBnCtx );
    BN_clear_free( pPubBn );

    return 0;
}

/* Convert an EC key's private key to a binary array. */
int ecies_privateKeyToBin(
    EC_KEY  *pEcKey,
    uint8  **ppPrivkey,
    uint32  *pPrivkLen
)
{
    const BIGNUM *pPriv = EC_KEY_get0_private_key( pEcKey );

    *pPrivkLen = BN_num_bytes( pPriv );
    *ppPrivkey = OPENSSL_malloc( *pPrivkLen );

    if (BN_bn2bin(pPriv, *ppPrivkey) != *pPrivkLen)
    {
        EC_ERROR("Failed to decode privkey\n");
    }

    return 0;
}

/* Convert a public key binary array to an EC point. */
EC_POINT *ecise_pubKeyBinToPoint(
    const EC_GROUP *pEcGroup,
    uint8          *pPubKey,
    uint32          pubKeyLen
)
{
    EC_POINT *pPubKeyPoint = NULL;
    BIGNUM   *pPubKeyBn;
    BN_CTX   *pPubKeyBnCtx;

    pPubKeyPoint = EC_POINT_new( pEcGroup );

    pPubKeyBn = BN_bin2bn(pPubKey, pubKeyLen, NULL);
    pPubKeyBnCtx = BN_CTX_new();
    BN_CTX_start( pPubKeyBnCtx );

    EC_POINT_bn2point(pEcGroup, pPubKeyBn, pPubKeyPoint, pPubKeyBnCtx);

    BN_CTX_end( pPubKeyBnCtx );
    BN_CTX_free( pPubKeyBnCtx );
    BN_clear_free( pPubKeyBn );

    return pPubKeyPoint;
}


uint32 ecise_shareKeyByX25519ByNwPubKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPubKey,
    uint32             nwPubKeyLen,
    uint8            **ppSkey
)
{
    EVP_PKEY_CTX *pCtx = NULL;
    EVP_PKEY *pEvKey = NULL;
    EVP_PKEY *pPeerkey = NULL;
    uint8  *pSkey = NULL;
    uint32  skeyLen = 0;
    
#if ECIES_USE_SPEC_PARAMETER
    /* assiged key*/
    char   *pPrivKeyStr = "c80949f13ebe61af4ebdbd293ea4f942696b9e815d7e8f0096bbf6ed7de62256";
    BIGNUM *pPrv = NULL;
    uint8  *pPrivKey = NULL;
    uint32  privKeyLen;

    BN_hex2bn(&pPrv, pPrivKeyStr);
    privKeyLen = BN_num_bytes( pPrv );
    pPrivKey = OPENSSL_malloc( privKeyLen );
    BN_bn2bin(pPrv, pPrivKey);
    pEvKey = EVP_PKEY_new_raw_private_key(
                 EVP_PKEY_X25519,
                 NULL,
                 pPrivKey,
                 privKeyLen
             );
    free( pPrivKey );
#else
    /* random generate key */
    pCtx = EVP_PKEY_CTX_new_id(NID_X25519, NULL);
    EVP_PKEY_keygen_init( pCtx );
    EVP_PKEY_keygen(pCtx, &pEvKey);
    EVP_PKEY_CTX_free( pCtx );
    pCtx = NULL;
#endif

    {
        uint8 buf[128];
        size_t len = 128;

        EVP_PKEY_get_raw_private_key(pEvKey, buf, &len);
        EC_DUMP("Eph. Private Key", buf, len);
    }

    pCtx = EVP_PKEY_CTX_new(pEvKey, NULL);
    EVP_PKEY_derive_init( pCtx );

    pPeerkey = EVP_PKEY_new_raw_public_key(
                   EVP_PKEY_X25519,
                   NULL,
                   pNwPubKey,
                   nwPubKeyLen
               );

    EVP_PKEY_derive_set_peer(pCtx, pPeerkey);

    EVP_PKEY_derive(pCtx, NULL, &skeyLen);
    pSkey = OPENSSL_malloc( skeyLen );
    EVP_PKEY_derive(pCtx, pSkey, &skeyLen);

    pEncrypInfo->pubkeyLen = ECIES_X25519_KEYLEN;
    pEncrypInfo->pPubKey = malloc( pEncrypInfo->pubkeyLen );

    EVP_PKEY_get_raw_public_key(
        pEvKey,
        pEncrypInfo->pPubKey,
        &(pEncrypInfo->pubkeyLen)
    );

    EVP_PKEY_CTX_free( pCtx );
    EVP_PKEY_free( pEvKey );
    EVP_PKEY_free( pPeerkey );

    *ppSkey = pSkey;

    return skeyLen;
}

uint32 ecies_shareKeyByX25519ByNwPrivKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPrivKey,
    uint32             nwPrivKeyLen,
    uint8            **ppSkey
)
{
    EVP_PKEY *pEvKey = NULL;
    EVP_PKEY_CTX *pCtx = NULL;
    EVP_PKEY *pPeerkey = NULL;
    uint8  *pSkey = NULL;
    uint32  skeyLen = 0;

    pEvKey = EVP_PKEY_new_raw_private_key(
                 EVP_PKEY_X25519,
                 NULL,
                 pNwPrivKey,
                 nwPrivKeyLen
             );

    pCtx = EVP_PKEY_CTX_new(pEvKey, NULL);
    EVP_PKEY_derive_init( pCtx );

    pPeerkey = EVP_PKEY_new_raw_public_key(
                   EVP_PKEY_X25519,
                   NULL,
                   pEncrypInfo->pPubKey,
                   pEncrypInfo->pubkeyLen
               );

    EVP_PKEY_derive_set_peer(pCtx, pPeerkey);

    EVP_PKEY_derive(pCtx, NULL, &skeyLen);

    pSkey = OPENSSL_malloc( skeyLen );

    EVP_PKEY_derive(pCtx, pSkey, &skeyLen);

    *ppSkey = pSkey;

    EVP_PKEY_CTX_free( pCtx );
    EVP_PKEY_free( pEvKey );
    EVP_PKEY_free( pPeerkey );

    return skeyLen;
}

uint32 ecies_shareKeyBySecp256r1ByNwPubKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPubKey,
    uint32             nwPubKeyLen,
    uint8            **ppSkey
)
{
    const EC_GROUP *pEcGroup = NULL;
    EC_POINT *pNwPubKeyPoint = NULL;
    EC_KEY *pEcKey = NULL;
    uint8  *pSkey = NULL;
    uint8  *pKeyBin = NULL;
    uint32  skeyLen = 0;

#if ECIES_USE_SPEC_PARAMETER  //use spec parameter
    /* assiged key*/
    char *p_str = "99798858A1DC6A2C68637149A4B1DBFD1FDFF5ADDD62A2142F06699ED7602529";

    BIGNUM *prv; 
    EC_POINT *pub;

    prv = BN_new(); 
    pEcKey = EC_KEY_new();
    pEcGroup = EC_GROUP_new_by_curve_name( NID_X9_62_prime256v1 );

    EC_KEY_set_group(pEcKey, pEcGroup);

    BN_hex2bn(&prv, p_str);

    pub = EC_POINT_new( pEcGroup );

    /* calculate the public key */ 
    EC_POINT_mul(pEcGroup, pub, prv, NULL, NULL, NULL); 

    /* add the private & public key to the EC_KEY structure */ 
    EC_KEY_set_private_key(pEcKey, prv); 
    EC_KEY_set_public_key(pEcKey, pub);

    pEcGroup = EC_KEY_get0_group( pEcKey );

    if ( prv )
    {
        BN_clear_free( prv );
    }
    
    if ( pub )
    {
        OPENSSL_free( pub );
    }
#else
    /* random generate key */
    pEcKey = EC_KEY_new_by_curve_name( NID_X9_62_prime256v1 );

    EC_KEY_generate_key( pEcKey );
    pEcGroup = EC_KEY_get0_group( pEcKey );
    EC_KEY_set_flags(pEcKey, EC_FLAG_COFACTOR_ECDH);
#endif

    {
        BIGNUM *pPrv = NULL; 
        uint8  *pPrivKey = NULL;
        uint32  privKeyLen;

        pPrv = EC_KEY_get0_private_key( pEcKey );
        privKeyLen = BN_num_bytes( pPrv );
        pPrivKey = OPENSSL_malloc( privKeyLen );
        BN_bn2bin(pPrv, pPrivKey);
        EC_DUMP("Eph. Private Key", pPrivKey, privKeyLen);
        free( pPrivKey );
        //BN_clear_free( pPrv );
    }

    /* Allocate a buffer to hold the shared symmetric key. */
    skeyLen = ((EC_GROUP_get_degree(pEcGroup) + 7) / 8);
    pSkey = OPENSSL_malloc( skeyLen );
    
    /* Convert the peer public key to an EC point. */
    pNwPubKeyPoint = ecise_pubKeyBinToPoint(pEcGroup, pNwPubKey, nwPubKeyLen);

    if ( pNwPubKeyPoint )
    {
        /* Generate the shared symmetric key (diffie-hellman primitive). */
        skeyLen = ECDH_compute_key(
                      pSkey,
                      skeyLen,
                      pNwPubKeyPoint,
                      pEcKey,
                      NULL
                  );
        OPENSSL_free( pNwPubKeyPoint );
    }

    /* Write the ephemeral key's public key to the output buffer. */
    ecies_publicKeyToBin(pEcKey, &pKeyBin, &(pEncrypInfo->pubkeyLen));

    pEncrypInfo->pPubKey = malloc( pEncrypInfo->pubkeyLen );
    if ( pEncrypInfo->pPubKey )
    {
        memcpy(pEncrypInfo->pPubKey, pKeyBin, pEncrypInfo->pubkeyLen);
        free( pKeyBin );
        EC_KEY_free( pEcKey );
    }
    else
    {
        EC_ERROR("fail to allocate memory\n");
        free( pKeyBin );
        free( pSkey );
        EC_KEY_free( pEcKey );
        return 0;
    }

    *ppSkey = pSkey;

    return skeyLen;
}

uint32 ecies_shareKeyBySecp256r1ByNwPrivKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPrivKey,
    uint32             nwPrivKeyLen,
    uint8            **ppSkey
)
{
    const EC_GROUP *pEcGroup = NULL;
    EC_POINT *pEphPubKeyPoint = NULL;
    EC_KEY   *pEcKey = NULL;
    BIGNUM   *pPrivKeyBn;
    uint8    *pSkey = NULL;
    uint32    skeyLen = 0;

    pEcKey = EC_KEY_new();
    pEcGroup = EC_GROUP_new_by_curve_name( NID_X9_62_prime256v1 );

    EC_KEY_set_group(pEcKey, pEcGroup);

    pPrivKeyBn = BN_bin2bn(pNwPrivKey, nwPrivKeyLen, NULL);

    EC_KEY_set_private_key(pEcKey, pPrivKeyBn);

    BN_clear_free( pPrivKeyBn );

    pEcGroup = EC_KEY_get0_group( pEcKey );

    /* Allocate a buffer to hold the shared symmetric key. */
    skeyLen = ((EC_GROUP_get_degree(pEcGroup) + 7) / 8);
    pSkey = OPENSSL_malloc( skeyLen );

    /* Convert the peer public key to an EC point. */
    pEphPubKeyPoint = ecise_pubKeyBinToPoint(
                          pEcGroup,
                          pEncrypInfo->pPubKey,
                          pEncrypInfo->pubkeyLen
                      );

    /* Generate the shared symmetric key (diffie-hellman primitive). */
    skeyLen = ECDH_compute_key(
                  pSkey,
                  skeyLen,
                  pEphPubKeyPoint,
                  pEcKey,
                  NULL
              );

    *ppSkey = pSkey;

    OPENSSL_free( pEphPubKeyPoint );
    EC_KEY_free( pEcKey );

    return skeyLen;
}

int ecies_aesCtr128AndSha256MacEncrypt(
    tEciesEncrypInfo *pEncrypInfo,
    uint8            *pDataKey,
    uint32            keLen,
    uint32            icbLen,
    uint32            kmLen,
    uint8            *pPlainMsg,
    uint32            plainMsgLen 
)
{
    const EVP_CIPHER *pCipher = EVP_aes_128_ctr();
    EVP_CIPHER_CTX *pEctx = NULL;
    uint8   enckey[keLen];
    uint8   icb[icbLen];
    uint8   mackey[kmLen];
    uint8   cipherMsg[32];
    uint8   mac[32];
    int     cipherMsgLen = 0;
    uint32  maclen;

    /* AES128-CTR encode Message */
    pEctx = EVP_CIPHER_CTX_new();

    memcpy(enckey, pDataKey, keLen);
    memcpy(icb, (pDataKey + keLen), icbLen);

    EVP_EncryptInit_ex(pEctx, pCipher, NULL, enckey, icb);
    EVP_EncryptUpdate(
        pEctx,
        cipherMsg,
        &cipherMsgLen,
        pPlainMsg,
        plainMsgLen
    );
    pEncrypInfo->cipherMsgLen = cipherMsgLen;
    EVP_EncryptFinal_ex(pEctx, cipherMsg, &cipherMsgLen);
    pEncrypInfo->cipherMsgLen += cipherMsgLen;

    pEncrypInfo->pCipherMsg = malloc( pEncrypInfo->cipherMsgLen );
    memcpy(pEncrypInfo->pCipherMsg, cipherMsg, pEncrypInfo->cipherMsgLen);

    /* calculate MAC */
    memcpy(mackey, (pDataKey + keLen + icbLen), kmLen);

    HMAC(
        EVP_sha256(),
        mackey,
        kmLen,
        pEncrypInfo->pCipherMsg,
        pEncrypInfo->cipherMsgLen,
        mac,
        &maclen
    );

    pEncrypInfo->macTagLen = ECIES_PARAM_MACLEN;
    pEncrypInfo->pMacTag = malloc( pEncrypInfo->macTagLen );
    memcpy(pEncrypInfo->pMacTag, mac, pEncrypInfo->macTagLen);

    EVP_CIPHER_CTX_free( pEctx );

    return 0;
}

uint32 ecies_aesCtr128AndSha256MacDecrypt(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pDataKey,
    uint32             keLen,
    uint32             icbLen,
    uint32             kmLen,
    uint8            **ppPlainMsg
)
{
    const EVP_CIPHER *pCipher = EVP_aes_128_ctr();
    EVP_CIPHER_CTX *pEctx = NULL;
    uint8   enckey[keLen];
    uint8   icb[icbLen];
    uint8   mackey[kmLen];
    uint8   plainMsg[32];
    uint32  plainMsgLen = 0;
    int     count = 0;
    uint8   macOut[kmLen];
    uint32  macOutLen;

    memcpy(enckey, pDataKey, keLen);
    memcpy(icb, (pDataKey + keLen), icbLen);
    memcpy(mackey, (pDataKey + keLen + icbLen), kmLen);

    HMAC(
        EVP_sha256(),
        mackey,
        kmLen,
        pEncrypInfo->pCipherMsg,
        pEncrypInfo->cipherMsgLen,
        macOut,
        &macOutLen
    );    

    if (memcmp(macOut, pEncrypInfo->pMacTag, ECIES_PARAM_MACLEN) != 0)
    {
        EC_ERROR("MAC verification failed\n");
        return 0;
    }

    pEctx = EVP_CIPHER_CTX_new();

    EVP_DecryptInit_ex(pEctx, pCipher, NULL, enckey, icb);
    EVP_DecryptUpdate(
        pEctx,
        plainMsg,
        &count,
        pEncrypInfo->pCipherMsg,
        pEncrypInfo->cipherMsgLen
    );
    plainMsgLen = count;
    EVP_DecryptFinal_ex(pEctx, plainMsg, &count);
    plainMsgLen += count;

    *ppPlainMsg = malloc( plainMsgLen );
    memcpy(*ppPlainMsg, plainMsg, plainMsgLen);

    EVP_CIPHER_CTX_free( pEctx );
    
    return plainMsgLen;
}

