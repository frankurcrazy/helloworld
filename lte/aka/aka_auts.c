#include "aka_f12345.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

bool MILENAGE_ALGORITHM = 1;

uint8 K[16] = {
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

uint8 OPc[16] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 RAND[16] = {
 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};

uint8 AUTS[14] = {
 0x35, 0xA8, 0x86, 0x4C, 0x1F, 0x67, 0x56, 0x78, 
 0x24, 0xB1, 0xA2, 0x1A, 0x4F, 0x6A
};

uint8 SQN[6] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AMF[2] = {
 0x00, 0x00
};

uint8 MAC[8];
uint8 XRES[16];
uint8 AUTN[16];
uint8 CK[16];
uint8 IK[16];
uint8 AK[6];

uint8 AUTS_S[14];
uint8 AK_S[6];
uint8 MAC_S[8];
uint8 SQN_S[6];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    if ( MILENAGE_ALGORITHM )
    {
        printf("3GPP AKA ... Milenage Algorithm\n\n");

        milenage_f1(K, OPc, RAND, SQN, AMF, MAC);
        //dump("MAC", MAC, 8);
        milenage_f2345(K, OPc, RAND, XRES, CK, IK, AK);
        //dump("AK", AK, 6);

        /* generate AUTN */
        xor(AUTN, SQN, AK, 6);
        memcpy(AUTN+6, AMF, 2);
        memcpy(AUTN+8, MAC, 8);
        dump("AUTN", AUTN, 16);


        milenage_f5star(K, OPc, RAND, AK_S);
        dump("AK_S", AK_S, 6);

        milenage_f1star(K, OPc, RAND, SQN, AMF, MAC_S);
        //dump("MAC_S", MAC_S, 8);
    }
    else
    {
        printf("3GPP AKA ... Test Algorithm\n\n");

        test_f1(K, NULL, RAND, SQN, AMF, MAC);
        //dump("MAC", MAC, 8);
        test_f2345(K, NULL, RAND, XRES, CK, IK, AK);
        //dump("AK", AK, 6);

        /* generate AUTN */
        xor(AUTN, SQN, AK, 6);
        memcpy(AUTN+6, AMF, 2);
        memcpy(AUTN+8, MAC, 8);
        dump("AUTN", AUTN, 16);


        test_f5star(K, NULL, RAND, AK_S);
        dump("AK_S", AK_S, 6);

        test_f1star(K, NULL, RAND, SQN, AMF, MAC_S);
        //dump("MAC_S", MAC_S, 8);
    }

    /* generate AUTS_S */
    xor(AUTS_S, SQN, AK_S, 6);
    memcpy(AUTS_S+6, MAC_S, 8);
    dump("AUTS_S", AUTS_S, 14);

    /* reveal SQN_S from AUTS */
    xor(SQN_S, AUTS, AK_S, 6);
    dump("SQN_S", SQN_S, 6);

    return 0;
}

