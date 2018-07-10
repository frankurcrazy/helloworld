#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void in_band(int bw, int nonanchor, int rb)
{
    printf("RB   Raster   Frequency offset   500Hz unit\n");
    if ( nonanchor )
    {
        if ((15 == bw) && (rb < 15))
        {
            dl_3MHz( rb );
        }
        else if ((25 == bw) && (rb < 25))
        {
            dl_5MHz( rb );
        }
        else if ((50 == bw) && (rb < 50))
        {
            dl_10MHz( rb, 0 );
        }
        else if ((75 == bw) && (rb < 75))
        {
            dl_15MHz( rb, 0 );
        }
        else if ((100 == bw) && (rb < 100))
        {
            dl_20MHz( rb, 0 );
        }
        else
        {
            printf("ERR: wrong bandwidth %d or RB %d\n", bw, rb);
        }
    }
    else
    {
        if (15 == bw)
        {
            dl_3MHz( 2  );
            dl_3MHz( 12 );
        }
        else if (25 == bw)
        {
            dl_5MHz(  2 );
            dl_5MHz(  7 );
            dl_5MHz( 17 );
            dl_5MHz( 22 );
        }
        else if (50 == bw)
        {
            dl_10MHz(  4, 0 );
            dl_10MHz(  9, 0 );
            dl_10MHz( 14, 0 );
            dl_10MHz( 19, 0 );
            dl_10MHz( 30, 0 );
            dl_10MHz( 35, 0 );
            dl_10MHz( 40, 0 );
            dl_10MHz( 45, 0 );
        }
        else if (75 == bw)
        {
            dl_15MHz(  2, 0 );
            dl_15MHz(  7, 0 );
            dl_15MHz( 12, 0 );
            dl_15MHz( 17, 0 );
            dl_15MHz( 22, 0 );
            dl_15MHz( 27, 0 );
            dl_15MHz( 32, 0 );
            dl_15MHz( 42, 0 );
            dl_15MHz( 47, 0 );
            dl_15MHz( 52, 0 );
            dl_15MHz( 57, 0 );
            dl_15MHz( 62, 0 );
            dl_15MHz( 67, 0 );
            dl_15MHz( 72, 0 );
        }
        else if (100 == bw)
        {
            dl_20MHz(  4, 0 );
            dl_20MHz(  9, 0 );
            dl_20MHz( 14, 0 );
            dl_20MHz( 19, 0 );
            dl_20MHz( 24, 0 );
            dl_20MHz( 29, 0 );
            dl_20MHz( 34, 0 );
            dl_20MHz( 39, 0 );
            dl_20MHz( 44, 0 );
            dl_20MHz( 55, 0 );
            dl_20MHz( 60, 0 );
            dl_20MHz( 65, 0 );
            dl_20MHz( 70, 0 );
            dl_20MHz( 75, 0 );
            dl_20MHz( 80, 0 );
            dl_20MHz( 85, 0 );
            dl_20MHz( 90, 0 );
            dl_20MHz( 95, 0 );
        }
        else
        {
            printf("ERR: wrong bandwidth %d\n", bw);
        }
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: inband_dl [OPTION]...\n");
    printf("  -h      Show the help message.\n");
    printf("\n");
    printf("For anchor carrier:\n");
    printf("  -b BW   Bandwidth in RBs (15, 25, 50, 75, 100).\n");
    printf("\n");
    printf("For non-anchor carrier:\n");
    printf("  -b BW   Bandwidth in RBs (15, 25, 50, 75, 100).\n");
    printf("  -n      Non-anchor carrier.\n");
    printf("  -r RB   RB index.\n");
    printf("\n");
}

/*
*  R13 36.104 Section 5.6 Channel bandwidth
*/
int main(int argc, char *argv[])
{
    int nonanchor = 0;
    int bw = 0;
    int rb = 0;
    int ch;

    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:r:nh")) != -1)
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
            case 'n':
                /* Non-anchor carrier */
                nonanchor = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    in_band(bw, nonanchor, rb);

    return 0;
}

