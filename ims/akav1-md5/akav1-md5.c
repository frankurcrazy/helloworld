#include <stdio.h>
#include "common.h"
#include "3gpp_aka.h"
#include "base64.h"
#include "digcalc.h"


#define TEST_VECTOR_1  (0)
#define TEST_VECTOR_2  (1)
#define TEST_VECTOR_3  (0)

#if (TEST_VECTOR_1)  /* Anritsu MD8475A IMS */
#include "test_vector_1.h"
#endif

#if (TEST_VECTOR_2)  /* iPhone Test algorithm */
#include "test_vector_2.h"
#endif

#if (TEST_VECTOR_3)  /* RFC 2617 example */
#include "test_vector_3.h"
#endif


int bin2hex(uint8 *inBuf, int inLen, char *outBuf, int outLen)
{
    char *pStr = outBuf;
    int i;

    if ((outLen + 1) <= (inLen * 2))
    {
        printf(
            "%s: output buffer is too small (%d <--> %d)\n",
            __func__,
            inLen,
            outLen
        );
        outBuf[0] = 0x0;
        return 0;
    }

    for (i=0; i<inLen; i++)
    {
        sprintf(pStr, "%02x", inBuf[i]);
        pStr += 2;
    }
    (*pStr) = 0x0;

    return strlen( outBuf );
}

void dump(char *name, const void *addr, unsigned int len)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (p == NULL)
    {
        printf("[dump] NULL pointer !!\n");
        printf("\n");
        return;
    }

    printf("[dump] %s  (len = %d)\n", name, len);
    for (i=0; i<len; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        #if 1
        printf("%02X ", p[i]);
        #else
        if ((p[i] > 0x1F) && (p[i] < 0x7F))
        {
            printf(" %c ", p[i]);
        }
        else
        {
            printf("%02X ", p[i]);
        }
        #endif
    }
    printf("\n");
    printf("[dump]\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    uint8 b64_buf[128];
    char  hex_buf[128];

    HASHHEX HA1;
    HASHHEX HEntity;
    HASHHEX Response;


    if (argc > 1)
    {
        b64_decode(argv[1], strlen(argv[1]), b64_buf, 128);
        dump("RAND", b64_buf,    16);
        dump("AUTN", b64_buf+16, 16);
        return 0;
    }


    #ifdef _USING_3GPP_AKA_
    memset(AKA_XRES, 0x00, 16);

    _3gpp_aka(
        AKA_K,     // IN
        NULL,      // IN
        AKA_OPC,   // IN
        AKA_RAND,  // IN
        AKA_SQN,   // IN
        AKA_AMF,   // IN
        AKA_MAC,   // OUT
        AKA_XRES,  // OUT
        AKA_CK,    // OUT
        AKA_IK,    // OUT
        AKA_AK,    // OUT
        AKA_AUTN,  // OUT
        milenage_algorithm  // IN
    );

    dump("RAND", AKA_RAND, 16);
    dump("AUTN", AKA_AUTN, 16);
    dump("XRES", AKA_XRES, 16);

    memcpy(b64_buf,    AKA_RAND, 16);
    memcpy(b64_buf+16, AKA_AUTN, 16);
    b64_encode(b64_buf, 32, pszNonce, 1024);

    printf("AKA Nonce:\n");
    printf("%s\n", pszNonce);
    printf("\n");

    memset(pszPass, 0x00, 20);
    memcpy(pszPass, AKA_XRES, 16);
    if ( milenage_algorithm )
    {
        pszPassLen = 8;
    }
    #endif


    bin2hex(pszPass, pszPassLen, hex_buf, 128);

    DigestCalcH(SIP_BODY, HEntity);

    printf("Username: %s\n", pszUser);
    printf("Realm: %s\n", pszRealm);
    printf("Nonce: %s\n", pszNonce);
    printf("URI: %s\n", pszURI);
    printf("Algorithm: %s\n", pszAlg);
    printf("CNonce: %s\n", pszCNonce);
    printf("QOP: %s\n", pszQop);
    printf("Count: %s\n", pszNonceCount);
    printf("Password: %s\n", hex_buf);
    printf("HEntity: %s\n", HEntity);

    DigestCalcHA1(
        pszAlg,     // IN
        pszUser,    // IN
        pszRealm,   // IN
        pszPass,    // IN
        pszPassLen,
        pszNonce,   // IN
        pszCNonce,  // IN
        HA1         // OUT
    );

    DigestCalcResponse(
        HA1,            // IN
        pszNonce,       // IN
        pszNonceCount,  // IN
        pszCNonce,      // IN
        pszQop,         // IN
        pszMethod,      // IN
        pszURI,         // IN
        HEntity,        // IN
        Response        // OUT
    );

    printf("\n");
    printf("HA1 = %s\n", HA1);
    printf("Response = %s\n", Response);
    printf("\n");

    return 0;
}
