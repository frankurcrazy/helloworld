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

uint8 SQN[6] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
};

uint8 AMF[2] = {
 0x00, 0x00
};

uint8 MAC[8];
uint8 XRES[16];
uint8 CK[16];
uint8 IK[16];
uint8 AK[6];
uint8 AUTN[16];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/*
* Input :  K, OPc, RAND, SQN, AMF
* Output:  AUTN, CK, IK, AK
*/
int main(int argc, char *argv[])
{
    if ( MILENAGE_ALGORITHM )
    {
        printf("3GPP AKA ... Milenage Algorithm\n\n");

        /* calculate MAC := f1(K, SQN, AMF, RAND) */
        milenage_f1(K, OPC, RAND, SQN, AMF, MAC);
        #if (_DEBUG_AKA_)
        dump("MAC", MAC, 8);
        #endif

        /* calculate XRES := f2(K, RAND) */
        milenage_f2345(K, OPC, RAND, XRES, CK, IK, AK);
        #if (_DEBUG_AKA_)
        dump("XRES", XRES, 8);
        dump("AK", AK, 6);
        #endif

        /* generate AUTN := (SQN) ^ AK || AMF || MAC */
        xor(AUTN, SQN, AK, 6);
        memcpy(AUTN+6, AMF, 2);
        memcpy(AUTN+8, MAC, 8);
    }
    else
    {
        printf("3GPP AKA ... Test Algorithm\n\n");

        /* calculate MAC := f1(K, SQN, AMF, RAND) */
        test_f1(K, NULL, RAND, SQN, AMF, MAC);
        #if (_DEBUG_AKA_)
        dump("MAC", MAC, 8);
        #endif

        /* calculate XRES := fs(K, RAND) */
        test_f2345(K, NULL, RAND, XRES, CK, IK, AK);
        #if (_DEBUG_AKA_)
        dump("XRES", XRES, 16);
        dump("AK", AK, 6);
        #endif

        /* generate AUTN := (SQN) ^ AK || AMF || MAC */
        xor(AUTN, SQN, AK, 6);
        memcpy(AUTN+6, AMF, 2);
        memcpy(AUTN+8, MAC, 8);
    }

    dump("K", K, 16);
    dump("RAND", RAND, 16);
    dump("AUTN", AUTN, 16);

    return 0;
}

