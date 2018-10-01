#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define CEIL(x, y)   (((x) + ((y) - 1)) / (y))
#define FLOOR(x, y)  ((x) / (y))


/*
* TS 36.213 Table 7.1.7.1-1: Modulation and TBS index table for PDSCH
*/
int tbs_index[] = {
     0,
     1,  2,  3,  4,  5,  6,  7,  8,  9,  9,
    10, 11, 12, 13, 14, 15, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26
};
int modulation[] = {
     2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  4,
     4,  4,  4,  4,  4,  4,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6
};

#include "tb_size.h"


void help(void)
{
    printf("Usage: downlink [OPTION]...\n");
    printf("\n");
    printf("  -m MCS   MCS index: 0 ~ 28.\n");
    printf("  -r RBs   RB nummber: 1 ~ 100.\n");
    printf("  -t       Two transport block (MIMO).\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.213 Section 7.1.7.2 Transport block size determination
 */
int main(int argc, char *argv[])
{
    double dataRate = 0;
    double mbps;
    int mcs = 28;
    int rb = 100;
    int mimo = 0;
    int I_TBS;
    int Qm;
    int bits;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "m:r:th")) != -1)
    {
        switch ( ch )
        {
            case 'm':
                mcs = atoi( optarg );
                break;
            case 'r':
                rb = atoi( optarg );
                break;
            case 't':
                mimo = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((mcs < 0) || (mcs > 28))
    {
        printf("ERR: incorrect MCS %d\n\n", mcs);
        return -1;
    }

    if ((rb < 1) || (rb > 100))
    {
        printf("ERR: incorrect RB number %d\n\n", rb);
        return -1;
    }

    I_TBS = tbs_index[mcs];
    Qm = modulation[mcs];
    bits = tb_size[ ((rb - 1) % 10) + (10 * I_TBS) + (270 * FLOOR((rb - 1), 10)) ];
    printf("MCS       = %d\n", mcs);
    printf("RB number = %d\n", rb);
    printf("TB size   = %d bits\n", bits);
    printf("Modulaton = %s\n", ((Qm == 2) ? "QPSK"  :
                                (Qm == 4) ? "16QAM" : "64QAM"));

    mbps = (double)(bits * 1000);
    if ( mimo ) mbps = (mbps * 2);

    if (mbps > 1000000)
    {
        dataRate = (mbps / 1000000);
        printf("Data rate = [1;33m%.1f Mbps[0m\n", dataRate);
    }
    else if (mbps > 1000)
    {
        dataRate = (mbps / 1000);
        printf("Data rate = [1;33m%.1f Kbps[0m\n", dataRate);
    }
    else
    {
        printf("Data rate = [1;33m%.1f bps[0m\n", dataRate);
    }
    printf("\n");

    return 0;
}

