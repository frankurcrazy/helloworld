#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sib_common.h"


/* SIB1-NB repetition control */
uint16  g_repIndex = 0;


void showSib1Nb(int startFrm)
{
    int blockIndex = 0;
    int i;
    int j;
    int k;


    printf("SIB1-NB occupied frames:\n");
    k = 0;
    for (i=0; i<1024; i++)
    {
        for (j=0; j<10; j++)
        {
            if (g_sib1Subframe[i] & (0x1 << j))
            {
                /* nf mod 256 */
                if ((i & 0xFF) == startFrm)
                {
                    g_repIndex = 0;
                }

                /* every other frame in 16 continuous frames */
                if (1 == startFrm)
                {
                    blockIndex = (((i + 15) & 0xF) >> 1);
                }
                else
                {
                    blockIndex = ((i & 0xF) >> 1);
                }

                if ((i != startFrm) && (k == 0)) printf("\n");

                /* start SIB1-NB transmission at this subframe */
                #if 1
                if (0 == blockIndex)
                {
                    printf("(%2d)", g_repIndex);
                    g_repIndex++;
                }
                #endif

                if ( !g_sib1Repetition[i] )
                {
                    printf(" [1;35m%4d[0m", i);
                }
                else
                {
                    printf(" %4d", i);
                }

                k = (k + 1) & 0x7;
            }
        }
    }
    printf("\n\n");
}

void help(void)
{
    printf("Usage: sib1nb [OPTION]...\n");
    printf("\n");
    printf("  -c cellId   Cell ID.\n");
    printf("  -s schInfo  SIB1-NB scheduling info (0 ~ 11).\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.331 Section 5.2.1.2a Scheduling for NB-IoT
*/
int main(int argc, char *argv[])
{
    int cellId = 0;
    int schInfo = 0;

    int repNum = 0;
    int tbSize = 0;
    int startFrm = 0;
    int N_SF;
    int N_Rep;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:s:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                cellId = atoi( optarg );
                break;
            case 's':
                schInfo = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((schInfo < 0) || (schInfo > 11))
    {
        printf("ERR: incorrect schInfo %d\n\n", schInfo);
        return -1;
    }

    sib1Nb_schedule(cellId, schInfo, &repNum, &tbSize, &startFrm);

    showSib1Nb( startFrm );

    N_SF = 8;
    N_Rep = repNum;
    printf("N_SF = [1;33m%d[0m\n", N_SF);
    printf("N_Rep = [1;33m%d[0m\n", N_Rep);
    printf("TB size = [1;33m%d[0m bits\n\n", tbSize);

    return 0;
}

