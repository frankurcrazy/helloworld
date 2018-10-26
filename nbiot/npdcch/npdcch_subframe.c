#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "npdcch_common.c"


void searchSpace(int type, int Rmax, int G, int alpha, int repNum, int verbose)
{
    int T;
    int alphaT;
    int k0;
    int k;
    int count;

    int R;
    int U;
    int r;
    int u;
    int b;


    k0 = npdcch_k0(Rmax, G, alpha, &T, &alphaT);
    if (k0 < 0)
    {
        printf("ERR: incorrect T = %d\n\n", T);
        return;
    }

    R = Rmax2R(type, Rmax, repNum);
    if (R <= 0)
    {
        printf("ERR: incorrect R = %d\n\n", R);
        return;
    }

    U = (Rmax / R);
    {
        count = 0;

        printf("             T = %d\n", T);
        printf("|<------------------------------>|\n");
        printf("     [1;32m+----------+[0m\n");
        printf("     [1;32m|  NPDCCH  |[0m\n");
        printf("     [1;32m+----------+[0m\n");
        printf("     |<-------->|\n");
        printf("       Rmax = %d (R = %d)\n", Rmax, R);
        printf("|---> [1;35mk0[0m\n");
        printf(" alphaT = %d\n", alphaT);
        printf("\n");

        if ( verbose )
        {
            printf("NS(L', R)k = \n");
        }
        for (k=0; k<10240; k++)
        {
            if ((k % T) == alphaT)
            {
                for (u=0; u<U; u++)
                {
                    for (r=0; r<R; r++)
                    {
                        if ( verbose )
                        {
                            if (((count % 10) == 0) && (count != 0))
                            {
                                printf("\n");
                            }
                            if (r > 0)
                            {
                                printf(" %5d", (k + (u * R) + r));
                            }
                            else
                            {
                                printf(" [1;32m%5d[0m", (k + (u * R) + r));
                            }
                        }
                        count++;
                    }
                }
            }
        }
        if ( verbose )
        {
            printf("\n\n");
        }

        printf("[1;35mkb:[0m\n");
        for (u=0; u<U; u++)
        {
            printf("  %d", (alphaT + (u * R)));
        }
        printf("\n");
        printf(" Number of subframe k0 = %d\n", (count / (U * R)));
        printf("\n");

        printf("[1;33mb:[0m\n");
        for (u=0; u<U; u++)
        {
            b = (u * R);
            printf("  %d", b);
        }
        printf("\n");
        printf(" Number of b = %d\n", U);
        printf("\n");
    }
}

void help(void)
{
    printf("Usage: npdcch_subframe [OPTION]...\n");
    printf("\n");
    printf("  -c type    Search space type.\n");
    printf("               0: USS\n");
    printf("               1: CSS type 1\n");
    printf("               2: CSS type 2\n");
    printf("  -r Rmax    Rmax (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048).\n");
    printf("  -g G       G (  3 -> 1.5,\n");
    printf("                  4 -> 2,\n");
    printf("                  8 -> 4,\n");
    printf("                 16 -> 8,\n");
    printf("                 32 -> 16,\n");
    printf("                 64 -> 32,\n");
    printf("                 96 -> 48,\n");
    printf("                128 -> 64).\n");
    printf("  -a alpha   Alpha offset (0 -> 0,\n");
    printf("                           1 -> 1/8,\n");
    printf("                           2 -> 1/4,\n");
    printf("                           3 -> 3/8).\n");
    printf("  -n repNum  DCI subframe repetition number (0, 1, 2, 3).\n");
    printf("  -v         Verbose log.\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.213 Section 16.6 Narrowband physical downlink control channel related procedures
*/
int main(int argc, char *argv[])
{
    int type = 0;
    int Rmax = 16;
    int G = 4;
    int alpha = 3;
    int repNum = 0;
    int verbose = 0;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:r:g:a:n:vh")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                type = atoi( optarg );
                break;
            case 'r':
                Rmax = atoi( optarg );
                break;
            case 'g':
                G = atoi( optarg );
                break;
            case 'a':
                alpha = atoi( optarg );
                break;
            case 'n':
                repNum = atoi( optarg );
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if (0 == type)
    {
        printf("[ UE-specific Search Space ]\n\n");
        searchSpace(0, Rmax, G, alpha, repNum, verbose);
    }
    else if (1 == type)
    {
        printf("[ Type-1 Common Search Space ]\n\n");

    }
    else if (2 == type)
    {
        printf("[ Type-2 Common Search Space ]\n\n");
        searchSpace(2, Rmax, G, alpha, repNum, verbose);
    }
    else
    {
        printf("Unknown search space type %d\n\n", type);
    }

    return 0;
}

