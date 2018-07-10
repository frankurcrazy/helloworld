#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
*  R13 36.212 Section 6.4.3.3 DCI Format N2
*/
int main(int argc, char *argv[])
{
    unsigned int  dci = 0;
    int pagingFlag;
    int directInfo;
    int iSF;
    int iMCS;
    int iRep;
    int dciRep;


    if (argc < 2)
    {
        printf("Usage: N0 <DCI in HEX string>\n");
        printf("\n");
        return 0;
    }

    sscanf(argv[1], "%x", &dci);
    dci &= 0x7FFF;  // 15-bit

    pagingFlag = ((dci >> 14) & 0x1);
    if ( pagingFlag )
    {
        iSF = ((dci >> 11) & 0x7);
        iMCS = ((dci >> 7) & 0xF);
        iRep = ((dci >> 3) & 0xF);
        dciRep = (dci & 0x7);
    }
    else
    {
        directInfo = ((dci >> 6) & 0xFF);
    }

    printf("\n");
    printf("DCI format N2  0x[1;35m%04X[0m (15-bit)\n", dci);
    printf("\n");
    printf("Flag for paging / direct indication  = %d\n", pagingFlag);
    if ( pagingFlag )
    {
    printf("Resource assignment (I_SF)           = %d\n", iSF);
    printf("Modulation and coding scheme (I_MCS) = %d\n", iMCS);
    printf("Repetition number (I_Rep)            = %d\n", iRep);
    printf("DCI subframe repetition number       = %d\n", dciRep);
    }
    else
    {
    printf("Direct indication information        = 0x%02X\n", directInfo);
    }
    printf("\n");

    return 0;
}

