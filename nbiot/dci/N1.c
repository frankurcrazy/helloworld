#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
*  R13 36.212 Section 6.4.3.2 DCI Format N1
*/
int main(int argc, char *argv[])
{
    unsigned int  dci = 0;
    int flagN1;
    int npdcchOrder;
    int nprachRep;
    int nprachScInd;
    int iDelay;
    int iSF;
    int iMCS;
    int iRep;
    int ndi;
    int harqRes;
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
    npdcchOrder = ((dci >> 21) & 0x1);
    if ( npdcchOrder )
    {
        nprachRep = ((dci >> 19) & 0x3);
        nprachScInd = ((dci >> 13) & 0x3F);
    }
    else
    {
        iDelay = ((dci >> 18) & 0x7);
        iSF = ((dci >> 15) & 0x7);
        iMCS = ((dci >> 11) & 0xF);
        iRep = ((dci >> 7) & 0xF);
        ndi = ((dci >> 6) & 0x1);
        harqRes = ((dci >> 2) & 0xF);
        dciRep = (dci & 0x3);
    }

    printf("\n");
    printf("DCI format N1  0x[1;35m%06X[0m (23-bit)\n", dci);
    printf("\n");
    printf("Flag for format N0 / N1               = %d\n", flagN1);
    printf("NPDCCH order indicator                = %d\n", npdcchOrder);
    if ( npdcchOrder )
    {
    printf("Starting number of NPRACH repetitions = %d\n", nprachRep);
    printf("Subcarrier indication of NPRACH       = %d\n", nprachScInd);
    }
    else
    {
    printf("Scheduling delay (I_Delay)            = %d\n", iDelay);
    printf("Resource assignment (I_SF)            = %d\n", iSF);
    printf("Modulation and coding scheme (I_MCS)  = %d\n", iMCS);
    printf("Repetition number (I_Rep)             = %d\n", iRep);
    printf("New data indicator                    = %d\n", ndi);
    printf("HARQ-ACK resource                     = %d\n", harqRes);
    printf("DCI subframe repetition number        = %d\n", dciRep);
    }
    printf("\n");

    return 0;
}

