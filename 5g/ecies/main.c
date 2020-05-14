#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ecies_api.h"



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

void help(void)
{
    printf("Usage: ecies_test [OPTION]...\n");
    printf("\n");
    printf("  -A         Profile A (CURVE25519)\n");
    printf("  -B         Profile B (SECP256R1)\n");
    printf("  -t INDEX   Generate network key:\n");
    printf("               0 - use spec. key\n");
    printf("               1 - use random key\n");
    printf("               2 - load from .der file\n");
    printf("  -f FILE    Private file (.der)\n");
    printf("\n");
}

int main(int argc, char * argv[])
{
    tEciesKeyInfo *pKeyInfo = NULL;
    tEciesEncrypInfo *pEncrypInfo = NULL;
    uint8 *pPlainMsg = NULL;
    uint8 *pDePlainMsg = NULL;
    uint32 decodeLen = 0 ;
    char *pNwPrivKey = NULL;
    char *pSchemeInput = "00012080F6";
    char *pArgStr = NULL;
    int   ecDomain = 0;
    int   itemNo = 0;
    int   ch;
    int   curve;    


    while ((ch=getopt(argc, argv, "abt:f:")) != -1)
    {
        switch ( ch )
        {
            case 'a':
                ecDomain = ECIES_PROFILE_A;
                break;
            case 'b':
                ecDomain = ECIES_PROFILE_B;
                break;
            case 't':
                itemNo = atoi( optarg );
                break;
            case 'f':
                pArgStr = optarg;
                break;                
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ((itemNo != ECIES_NW_KEY_ASSIGNED) &&
        (itemNo != ECIES_NW_KEY_RANDOM) &&
        (itemNo != ECIES_NW_KEY_DER))
    {
        printf("ERR: incorrect itemNo %d\n", itemNo);
        return 0;
    }
    if ((ECIES_NW_KEY_DER == itemNo) && (NULL == pArgStr))
    {
        printf("ERR: pArgStr is NULL\n");
        return 0;
    }

    if (ECIES_PROFILE_A == ecDomain)
    {
        curve = EC_DOMAIN_CURVE25519;
        pNwPrivKey = "c53c22208b61860b06c62e5406a7b330c2b577aa5558981510d128247d38bd1d"; 
        printf("[ [1;36mECIES Profile A[0m - use CURVE25519 ]\n\n");
    }
    else if (ECIES_PROFILE_B == ecDomain)
    {
        curve = EC_DOMAIN_SECP256R1;
        pNwPrivKey = "F1AB1074477EBCC7F554EA1C5FC368B1616730155E0041AC447D6301975FECDA"; 
        printf("[ [1;36mECIES Profile B[0m - use SECP256R1 ]\n\n");
    }
    else
    {
        printf("ERR: incorrect profile %d\n", ecDomain);
        return 0;
    }
    

    pPlainMsg = malloc(strlen(pSchemeInput) / 2);
    HEX_STRING_TO_BYTE_ARRAY(
        pSchemeInput,
        strlen(pSchemeInput),
        pPlainMsg,
        (strlen(pSchemeInput) / 2)
    );

    printf("------------------- Load ECIES Network Key -----------------\n");
    /* ECIES receiver loads the EC key */
    pKeyInfo = ecies_setKeyMode(
                   itemNo,
                   curve,
                   pArgStr,
                   pNwPrivKey
               );

    printf("------------------- ECIES Encode Message -------------------\n");
    pEncrypInfo = ecies_encodeMsg(
                      pKeyInfo->profile,
                      pKeyInfo->pNwPubKey,
                      pKeyInfo->nwPubkeyLen,
                      pPlainMsg,
                      (strlen(pSchemeInput) / 2)
                  );

    printf("[1;31mScheme Output[0m:\n");
    mem_dump(pEncrypInfo->pPubKey, pEncrypInfo->pubkeyLen);
    mem_dump(pEncrypInfo->pCipherMsg, pEncrypInfo->cipherMsgLen);
    mem_dump(pEncrypInfo->pMacTag, pEncrypInfo->macTagLen);
    printf("\n");

    printf("------------------- ECIES Decode Cipher Message ------------\n");
    decodeLen = ecies_decodeMsg(
                    pEncrypInfo,
                    pKeyInfo->pNwPrivKey,
                    pKeyInfo->nwPrivkeyLen,
                    &pDePlainMsg  
                );


    ecies_freeEncrypInfo( pEncrypInfo );
    ecies_freeKeyInfo( pKeyInfo );

    free( pPlainMsg );
    if ( pDePlainMsg )
    {
        free( pDePlainMsg );
    }

    return 0;
}




