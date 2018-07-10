#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void in_band(int bw, int rb)
{
    printf("RB   Frequency offset   500Hz unit\n");
    {
        if ((15 == bw) && (rb < 15))
        {
            ul_3MHz( rb );
        }
        else if ((25 == bw) && (rb < 25))
        {
            ul_5MHz( rb );
        }
        else if ((50 == bw) && (rb < 50))
        {
            ul_10MHz( rb );
        }
        else if ((75 == bw) && (rb < 75))
        {
            ul_15MHz( rb );
        }
        else if ((100 == bw) && (rb < 100))
        {
            ul_20MHz( rb );
        }
        else
        {
            printf("ERR: wrong bandwidth %d or RB %d\n", bw, rb);
        }
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: inband_ul [OPTION]...\n");
    printf("  -b BW   Bandwidth in RBs (15, 25, 50, 75, 100).\n");
    printf("  -r RB   RB index.\n");
    printf("  -h      Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.104 Section 5.6 Channel bandwidth
*/
int main(int argc, char *argv[])
{
    int bw = 0;
    int rb = 0;
    int ch;

    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                /* Bandwidth */
                bw = atoi( optarg );
                break;
            case 'r':
                /* RB index */
                rb = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    in_band(bw, rb);

    return 0;
}

