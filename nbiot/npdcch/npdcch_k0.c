#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "npdcch_common.c"


void show_all_npdcch_k0(void)
{
    int T;
    int alphaT;
    int Rmax;
    int G;
    int alpha;
    int i;
    int j;
    int k;

    for (i=0; i<8; i++)
    {
        G = _G[i];

        printf("G = %s\n", _GStr[i]);
        for (j=0; j<12; j++)
        {
            Rmax = _Rmax[j];

            T = ((Rmax * G) / 2);
            if (T >= 4)
            {
                printf("  T = %d (Rmax %d)\n", T, Rmax);
                printf("    k0 = ");
                for (k=0; k<4; k++)
                {
                    alpha = _alpha[k];
                    alphaT = ((alpha * T) / 8);
                    if (alphaT < 10240)
                    {
                        printf(" [1;33m%d[0m ", alphaT);
                    }
                    else
                    {
                        printf(" %d ", alphaT);
                    }
                }
                printf("\n");
            }
        }
        printf("\n");
    }
    printf("\n");
}


void help(void)
{
    printf("Usage: npdcch_k0 [OPTION]...\n");
    printf("\n");
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
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.213 Section 16.6 Narrowband physical downlink control channel related procedures
*/
int main(int argc, char *argv[])
{
    int Rmax = 16;
    int G = 4;
    int alpha = 3;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "r:g:a:h")) != -1)
    {
        switch ( ch )
        {
            case 'r':
                Rmax = atoi( optarg );
                break;
            case 'g':
                G = atoi( optarg );
                break;
            case 'a':
                alpha = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    show_all_npdcch_k0();

    return 0;
}

