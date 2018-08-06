#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"


int mapping(int N_UL_RB, int m, int ns)
{
    int nPRB;

    if (0 == ((m + ns) % 2))
    {
        nPRB = DIV_FLOOR(m, 2);
    }
    else
    {
        nPRB = N_UL_RB - 1 - DIV_FLOOR(m, 2);
    }

    return nPRB;
}

void help(void)
{
    printf("Usage: q_determine [OPTION]...\n");
    printf("\n");
    printf("  -a           Show all RB mapping.\n");
    printf("  -b RBs       Bandwidth: 6, 15, 25, 50, 75, 100.\n");
    printf("  -s Subframe  Subframe number (0 ~ 9).\n");
    printf("  -m m         m value.\n");
    printf("\n");
    printf("  -h           Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.211 Section 5.4.3 Mapping to physical resources
 */
int main(int argc, char *argv[])
{
    int N_UL_RB = 100;
    int subframe = 1;
    int m = 3;
    int aFlag = 0;

    int nPRB[100][2];
    int ns[2];
    int rb;
    int i;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "ab:s:m:h")) != -1)
    {
        switch ( ch )
        {
            case 'a':
                aFlag = 1;
                break;
            case 'b':
                N_UL_RB = atoi( optarg );
                break;
            case 's':
				subframe = atoi( optarg );
                break;
            case 'm':
                m = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ( aFlag )
    {
        ns[0] = subframe * 2;
        ns[1] = (ns[0] + 1);
        for (m=0; m<N_UL_RB; m++)
        {
            rb = mapping(N_UL_RB, m, ns[0]);
            nPRB[rb][0] = m;

            rb = mapping(N_UL_RB, m, ns[1]);
            nPRB[rb][1] = m;
        }

        printf("        slot[%2d]  slot[%2d]\n", ns[0], ns[1]);
        rb = (N_UL_RB - 1);
        for (i=0; i<N_UL_RB; i++)
        {
            printf("RB[%2d]       %2d        %2d\n", rb, nPRB[rb][0], nPRB[rb][1]);
            rb--;
        }
        printf("\n");
    }
    else
    {
        ns[0] = subframe * 2;
        ns[1] = (ns[0] + 1);
        nPRB[0][0] = mapping(N_UL_RB, m, ns[0]);
        nPRB[0][1] = mapping(N_UL_RB, m, ns[1]);

        printf("N_UL_RB = %d\n", N_UL_RB);
        printf("subframe = %d\n", subframe);
        printf("m = %d\n", m);
        printf("\n");

        if (nPRB[0][0] > nPRB[0][1])
        {
            printf("    slot[%2d]  slot[%2d]\n", ns[0], ns[1]);
            printf("RB       %2d\n", nPRB[0][0]);
            printf("RB                 %2d\n", nPRB[0][1]);
        }
        else
        {
            printf("    slot[%2d]  slot[%2d]\n", ns[0], ns[1]);
            printf("RB                 %2d\n", nPRB[0][1]);
            printf("RB       %2d\n", nPRB[0][0]);
        }
        printf("\n");
    }

    return 0;
}

