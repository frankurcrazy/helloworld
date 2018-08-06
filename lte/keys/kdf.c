#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include "kdf.h"


#define HMAC_SHA256_SIZE  32


int kdf(
    uint8  *pKey,
    uint16  keyLen,
    uint8  *pS,
    uint16  sLen,
    uint8  *pOut
)
{
    uint32 resultLen = HMAC_SHA256_SIZE;

    HMAC(EVP_sha256(), pKey, keyLen, pS, sLen, pOut, &resultLen);

    //mem_dump("hmac-sha256", pOut, resultLen);

    return 0;
}

void mem_dump(char *name, const void *addr, unsigned int len)
{
    unsigned char *pByte = (unsigned char *)addr;
    unsigned int   i;

    if (addr == NULL)
    {
        printf("%s: NULL\n", name);
        printf("\n");
        return;
    }

    printf("[1;33m%s[0m:\n", name);
    for(i=0; i<len; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf(" %02X", pByte[i]);
    }
    printf("\n");
    printf(" (%u bytes)\n", len);
    printf("\n");
}


