#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


/* antennaNum x L_EPDCCH_Start */
int CRS[5][5] = {
  {  0,  0,  0,  0,  0 },
  { 16, 14, 14, 14, 14 },
  { 32, 28, 28, 28, 28 },
  {  0,  0,  0,  0,  0 },
  { 48, 44, 40, 40, 40 },
};

int main(int argc, char *argv[])
{
    int antennaNum = 1;
    int l_EPDCCH_Start = 3;
    int nEPDCCH;


    if (argc > 2)
    {
        antennaNum = atoi( argv[1] );
        l_EPDCCH_Start = atoi( argv[2] );
    }
    else if (argc > 1)
    {
        antennaNum = atoi( argv[1] );
    }

    nEPDCCH = (144 - (l_EPDCCH_Start * 12) - CRS[antennaNum][l_EPDCCH_Start]);

    printf("antennaNum = %d\n", antennaNum);
    printf("l_EPDCCH_Start = %d\n", l_EPDCCH_Start);
    printf("\n");
    printf("nEPDCCH = %d\n", nEPDCCH);
    printf("\n");


    return 0;
}
