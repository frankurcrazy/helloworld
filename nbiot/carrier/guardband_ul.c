#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void guard_band(int bw)
{
    printf("RB   Frequency offset   500Hz unit\n");
    if (25 == bw)
    {
        ul_5MHz( -1 );
        ul_5MHz( 25 );
    }
    else if (50 == bw)
    {
        ul_10MHz( -2 );
        ul_10MHz( -1 );
        ul_10MHz( 51 );
        ul_10MHz( 50 );
    }
    else if (75 == bw)
    {
        ul_15MHz( -4 );
        ul_15MHz( -3 );
        ul_15MHz( -2 );
        ul_15MHz( -1 );
        ul_15MHz( 75 );
        ul_15MHz( 76 );
        ul_15MHz( 77 );
        ul_15MHz( 78 );
    }
    else if (100 == bw)
    {
        ul_20MHz( -5  );
        ul_20MHz( -4  );
        ul_20MHz( -3  );
        ul_20MHz( -2  );
        ul_20MHz( -1  );
        ul_20MHz( 100 );
        ul_20MHz( 101 );
        ul_20MHz( 102 );
        ul_20MHz( 103 );
        ul_20MHz( 104 );
    }
    else
    {
        printf("ERR: wrong bandwidth %d\n", bw);
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: guardband_ul [OPTION]...\n");
    printf("  -b BW   Bandwidth in RBs (25, 50, 75, 100).\n");
    printf("  -h      Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.104 Section 5.6 Channel bandwidth
*/
int main(int argc, char *argv[])
{
    int bw = 0;
    int ch;

    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                /* Bandwidth */
                bw = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    guard_band( bw );

    return 0;
}

