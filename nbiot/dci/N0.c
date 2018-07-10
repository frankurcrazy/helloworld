#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
*  R13 36.212 Section 6.4.3.1 DCI Format N0
*/
int main(int argc, char *argv[])
{
    unsigned int  dci = 0;
    int flagN1;
    int iSC;
    int iRU;
    int iDelay;
    int iMCS;
    int rvDCI;
    int iRep;
    int ndi;
    int dciRep;


    if (argc < 2)
    {
        printf("Usage: N0 <DCI in HEX string>\n");
        printf("\n");
        return 0;
    }

    sscanf(argv[1], "%x", &dci);
    dci &= 0x7FFFFF;  // 23-bit

    flagN1 = ((dci >> 22) & 0x1);
    iSC = ((dci >> 16) & 0x3F);
    iRU = ((dci >> 13) & 0x7);
    iDelay = ((dci >> 11) & 0x3);
    iMCS = ((dci >> 7) & 0xF);
    rvDCI = ((dci >> 6) & 0x1);
    iRep = ((dci >> 3) & 0x7);
    ndi = ((dci >> 2) & 0x1);
    dciRep = (dci & 0x3);

    printf("\n");
    printf("DCI format N0  0x[1;35m%06X[0m (23-bit)\n", dci);
    printf("\n");
    printf("Flag for format N0 / N1              = %d\n", flagN1);
    printf("Subcarrier indication (I_SC)         = %d\n", iSC);
    printf("Resource assignment (I_RU)           = %d\n", iRU);
    printf("Scheduling delay (I_Delay)           = %d\n", iDelay);
    printf("Modulation and coding scheme (I_MCS) = %d\n", iMCS);
    printf("Redundancy version                   = %d\n", rvDCI);
    printf("Repetition number (I_Rep)            = %d\n", iRep);
    printf("New data indicator                   = %d\n", ndi);
    printf("DCI subframe repetition number       = %d\n", dciRep);
    printf("\n");

    return 0;
}

