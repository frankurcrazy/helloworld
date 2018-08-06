#include "aka_f12345.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define _DEBUG_AKA_  (0)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

bool MILENAGE_ALGORITHM = 1;

uint8 K[16] = {
 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0x61, 0x08, 0x63, 0x64, 0x65, 0x66
};

uint8 OPC[16] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 RAND[16] = {
 0xEF, 0x35, 0xA6, 0xC5, 0x64, 0x2B, 0x27, 0x78,
 0xDF, 0x26, 0xEF, 0x31, 0x17, 0x85, 0xF8, 0xDD
};

uint8 AUTN[16] = {
 0xA2, 0x3F, 0x9E, 0x79, 0xC5, 0x8E, 0x00, 0x00,
 0x6E, 0xA6, 0x46, 0x94, 0xE4, 0x7F, 0x55, 0xE9
};

uint8 SQN[6];
uint8 AMF[2];
uint8 MAC[8];
uint8 XMAC[8];
uint8 RES[16];
uint8 CK[16];
uint8 IK[16];
uint8 AK[6];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/*
* Input :  K, OPc, RAND, AUTN
* Output:  RES, CK, IK, AK
*/
int main(int argc, char *argv[])
{
    if ( MILENAGE_ALGORITHM )
    {
        printf("3GPP AKA ... Milenage Algorithm\n\n");

        /* calculate AK := f5(K, RAND) */
        milenage_f2345(K, OPC, RAND, RES, CK, IK, AK);
        #if (_DEBUG_AKA_)
        dump("AK", AK, 6);
        dump("RES", RES, 8);
        #endif

        /* get SQN, AMF and MAC from AUTN */
        xor(SQN, AUTN, AK, 6);
        memcpy(AMF, AUTN+6, 2);
        memcpy(MAC, AUTN+8, 8);

        /* calculate XMAC := f1(K, SQN, AMF, RAND) */
        milenage_f1(K, OPC, RAND, SQN, AMF, XMAC);
    }
    else
    {
        printf("3GPP AKA ... Test Algorithm\n\n");

        /* calculate AK := f5(K, RAND) */
        test_f2345(K, NULL, RAND, RES, CK, IK, AK);
        #if (_DEBUG_AKA_)
        dump("AK", AK, 6);
        dump("RES", RES, 16);
        #endif

        /* get SQN, AMF and MAC from AUTN */
        xor(SQN, AUTN, AK, 6);
        memcpy(AMF, AUTN+6, 2);
        memcpy(MAC, AUTN+8, 8);

        /* calculate XMAC := f1(K, SQN, AMF, RAND) */
        test_f1(K, NULL, RAND, SQN, AMF, XMAC);
    }

    dump("SQN", SQN, 6);
    dump("AMF", AMF, 2);
    dump("MAC", MAC, 8);
    dump("XMAC", XMAC, 8);

    return 0;
}

