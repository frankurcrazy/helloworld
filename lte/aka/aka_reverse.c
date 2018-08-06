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

/*
* K: 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
*/

uint8 RAND[16] = {
 0x23, 0x40, 0xA4, 0xCD, 0xC0, 0x77, 0x91, 0x4B,
 0x39, 0xA0, 0x56, 0x0B, 0x82, 0xD3, 0x84, 0xE7
};

uint8 AUTN[16] = {
 0xFE, 0x84, 0x22, 0xF7, 0x3C, 0xB1, 0x00, 0x00,
 0x23, 0x51, 0x86, 0xFE, 0x84, 0x22, 0xF7, 0x3C
};

uint8 SQN[6] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AMF[2] = {
 0x00, 0x00
};

uint8 RES[16] = {
 0x23, 0x51, 0x86, 0xFE, 0x84, 0x22, 0xF7, 0x3C,
 0xB1, 0x39, 0xFC, 0xB0, 0x4E, 0x0E, 0x6A, 0x18
};

uint8 K[16];
uint8 MAC[8];
uint8 AK[6];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void show_K(uint8 *K, int offset, int size)
{
    int start = offset;
    int end = (offset + size - 1);
    int i;

    printf("K[%d:%d]\n", start, end);

    for (i=0; i<offset; i++)
    {
        printf("xx ");
    }

    printf("[1;31m");
    for (i=start; i<(end+1); i++)
    {
        printf("%02X ", K[i]);
    }
    printf("[0m");

    for (i=end; i<16; i++)
    {
        printf("xx ");
    }

    printf("\n\n");
}

int main(int argc, char *argv[])
{
    uint8 xdout[16];
    uint8 cdout[16];

    memset(xdout, 0x00, 16);
    memcpy(cdout,   SQN, 6);
    memcpy(cdout+6, AMF, 2);

    /*
    *     AUTN[0:5] = SQN ^ AK
    * ==> AK = AUTN[0:5] ^ SQN
    *
    *     AK = f5(K, RAND)
    *        = K[3:8] ^ RAND[3:8]
    * ==> K[3:8] = AK ^ RAND[3:8]
    */
    memset(K, 0x00, 16);
    xor(AK, AUTN, SQN, 6);
    dump("AK", AK, 6);
    xor(K+3, AK, RAND+3, 6);
    show_K(K, 3, 6);

    /*
    *     RES = f2(K, RAND)
    *         = K[0:7] ^ RAND[0:7]
    * ==> K[0:7] = RES ^ RAND[0:7]
    */
    memset(K, 0x00, 16);
    xor(K, RES, RAND, 8);
    show_K(K, 0, 8);

    /*
    *    AUTN[6:7] = AMF
    *     AUTN[8:15] = MAC
    *     MAC = f1(K, RAND, SQN, AMF)
    *         = (K[0:7] ^ RAND[0:7]) ^ (SQN + AMF)
    * ==> K[0:7] = MAC ^ (SQN + AMF) ^ RAND[0:7]
    */
    memset(K, 0x00, 16);
    memcpy(MAC, AUTN+8, 8);
    dump("MAC", MAC, 8);
    xor(xdout, MAC, cdout, 8);
    xor(K, xdout, RAND, 8);
    show_K(K, 0, 8);

    return 0;
}

