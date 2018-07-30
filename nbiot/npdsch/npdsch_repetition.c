#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void help(void)
{
    printf("Usage: npdsch_repetition [OPTION]...\n");
    printf("\n");
    printf("  -b        NPDSCH carries BCCH.\n");
    printf("  -s N_SF   Number of subframe (1, 2, 3, 4, 5, 6, 8, 10).\n");
    printf("  -r N_Rep  Number of repetition\n");
    printf("            (1, 2, 4, 8, 16, 32, 64, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.211 Section 10.2.3 Narrowband physical downlink shared channel
*/
int main(int argc, char *argv[])
{
    int BCCH = 0;
    int N_SF = 1;
    int N_Rep = 1;
    int repMin;
    int ch;
    int i;
    int j;
    int k;


    opterr = 0;
    while ((ch=getopt(argc, argv, "bs:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                BCCH = 1;
                break;
            case 's':
                N_SF = atoi( optarg );
                break;
            case 'r':
                N_Rep = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ((N_SF != 1) &&
        (N_SF != 2) &&
        (N_SF != 3) &&
        (N_SF != 4) &&
        (N_SF != 5) &&
        (N_SF != 6) &&
        (N_SF != 8) &&
        (N_SF != 10))
    {
        printf("ERR: incorrect N_SF (%d)\n\n", N_SF);
        return -1;
    }

    if ((N_Rep !=    1) &&
        (N_Rep !=    2) &&
        (N_Rep !=    4) &&
        (N_Rep !=    8) &&
        (N_Rep !=   16) &&
        (N_Rep !=   32) &&
        (N_Rep !=   64) &&
        (N_Rep !=  128) &&
        (N_Rep !=  192) &&
        (N_Rep !=  256) &&
        (N_Rep !=  384) &&
        (N_Rep !=  512) &&
        (N_Rep !=  768) &&
        (N_Rep != 1024) &&
        (N_Rep != 1536) &&
        (N_Rep != 2048))
    {
        printf("ERR: incorrect N_Rep (%d)\n\n", N_Rep);
        return -1;
    }

    printf("Total [1;33m%d[0m subframe(s):\n", (N_SF * N_Rep));
    if ( BCCH )
    {
        for (j=0; j<N_Rep; j++)
        {
            for (i=0; i<N_SF; i++)
            {
                if (0 == j)
                {
                    if (i > 0)
                    {
                        printf(" [1;32m(n+%d)[0m", i);
                    }
                    else
                    {
                        printf(" [1;32m(n  )[0m");
                    }
                }
                else
                {
                    if (i > 0)
                    {
                        printf(" (n+%d)", i);
                    }
                    else
                    {
                        printf(" (n  )");
                    }
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    else
    {
        repMin = ((N_Rep > 4) ? 4 : N_Rep);
        for (j=0; j<(N_Rep / repMin); j++)
        {
            for (i=0; i<N_SF; i++)
            {
                if (0 == j)
                {
                    if (i > 0)
                    {
                        printf(" [1;32m(n+%d)[0m", i);
                    }
                    else
                    {
                        printf(" [1;32m(n  )[0m");
                    }
                }
                else
                {
                    if (i > 0)
                    {
                        printf(" (n+%d)", i);
                    }
                    else
                    {
                        printf(" (n  )");
                    }
                }

                for (k=0; k<(repMin - 1); k++)
                {
                    if (i > 0)
                    {
                        printf(" (n+%d)", i);
                    }
                    else
                    {
                        printf(" (n  )");
                    }                
                }
                if (k > 0)
                {
                    printf("\n");
                }
            }
        }
    }
    printf("\n");

    return 0;
}

