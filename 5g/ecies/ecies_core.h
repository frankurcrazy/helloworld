#ifndef __ECIES_CORE_H__
#define __ECIES_CORE_H__

#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define ECIES_X25519_KEYLEN (32)

#define ECIES_PARAM_ENCKEYLEN  (16)
#define ECIES_PARAM_ICBLEN     (16)
#define ECIES_PARAM_MACKEYLEN  (32)
#define ECIES_PARAM_MACLEN     (8)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void *ecies_setNwKeyByAssign(int curve, char *pPrivKeyStr);
void *ecies_setNwKeyViaRandom(int curve);
void *ecies_setNwKeyByDer(int curve, char *pFilename);


int ecies_publicKeyToBin(
    EC_KEY  *pEcKey,
    uint8  **ppPubkey,
    uint32  *pPubkLen
);

int ecies_privateKeyToBin(
    EC_KEY  *pEcKey,
    uint8  **ppPrivkey,
    uint32  *pPrivkLen
);

EC_POINT *ecise_pubKeyBinToPoint(
    const EC_GROUP *pEcGroup,
    uint8          *pPubKey,
    uint32          pubKeyLen
);


uint32 ecise_shareKeyByX25519ByNwPubKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPubKey,
    uint32             nwPubKeyLen,
    uint8            **ppSkey
);

uint32 ecies_shareKeyByX25519ByNwPrivKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPrivKey,
    uint32             nwPrivKeyLen,
    uint8            **ppSkey
);

uint32 ecies_shareKeyBySecp256r1ByNwPubKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPubKey,
    uint32             nwPubKeyLen,
    uint8            **ppSkey
);

uint32 ecies_shareKeyBySecp256r1ByNwPrivKey(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pNwPrivKey,
    uint32             nwPrivKeyLen,
    uint8            **ppSkey
);


int ecies_aesCtr128AndSha256MacEncrypt(
    tEciesEncrypInfo *pEncrypInfo,
    uint8            *pDataKey,
    uint32            keLen,
    uint32            icbLen,
    uint32            kmLen,
    uint8            *pPlainMsg,
    uint32            plainMsgLen 
);

uint32 ecies_aesCtr128AndSha256MacDecrypt(
    tEciesEncrypInfo  *pEncrypInfo,
    uint8             *pDataKey,
    uint32             keLen,
    uint32             icbLen,
    uint32             kmLen,
    uint8            **ppPlainMsg
);


#endif
