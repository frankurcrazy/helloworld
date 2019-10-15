#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


static void _pdcchCandidates(int Yk, int mP, int N_CCE_k, int L)
{
    int cceIndex;
    int i;

    printf(" {");

    for (i = 0; i < L; i++)
    {
        cceIndex = L * ((Yk + mP) % DIV_FLOOR(N_CCE_k, L)) + i;
        if (0 == i)
        {
            printf(" [1;33m%2d[0m", cceIndex);
        }
        else
        {
            printf(" %2d", cceIndex);
        }
    }

    printf(" }\n");
}

void help(void)
{
    printf("Usage: cce [OPTION]...\n");
    printf("\n");
    printf("  -b RBs        Bandwidth: 6, 15, 25, 50, 75, 100.\n");
    printf("  -c CFI        CFI value: 1, 2, 3.\n");
    printf("  -s SUBFRAME   Subframe number.\n");
    printf("  -r RNTI       RNTI value.\n");
    printf("  -m            PHICH mi value.\n");
    printf("\n");
    printf("  -h            Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.213 Section 9.1.1 PDCCH Assignment Procedure
 */
int main(int argc, char *argv[])
{
    int N_DL_RB = 100;
    int CFI = 3;
    int subframe = 0;
    int RNTI = 12;
    int mi = 1;
    int N_group_PHICH;

    int Yk;
    int mP;
    int m;
    int N_CCE_k;
    int N_REG;
    int L;
    int M_L;
    int ch;
    int i;


    opterr = 0;
    while ((ch=getopt(argc, argv, "b:c:s:r:m:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                N_DL_RB = atoi( optarg );
                break;
            case 'c':
                CFI = atoi( optarg );
                break;
            case 's':
                subframe = atoi( optarg );
                break;
            case 'r':
                RNTI = atoi( optarg );
                break;
            case 'm':
                mi = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    N_group_PHICH = DIV_CEIL((N_DL_RB / 8), 2);  // Ng = 1/2
    N_REG  = ((2 + (CFI - 1) * 3) * N_DL_RB);
    N_REG -= 4;  // PCFICH
    N_REG -= (3 * mi * N_group_PHICH);  // PHICH
    N_CCE_k = DIV_FLOOR(N_REG, 9);
    printf("N_CCE = [1;32m%d[0m\n", N_CCE_k);
    printf("\n");


    // Common search space
    {
        int L_common[]   = {4, 8};
        int M_L_common[] = {4, 2};

        printf("Common search space:\n");
        Yk = 0;

        for (i = 0; i < 2; i++)
        {
            L = L_common[i];
            M_L = M_L_common[i];
            printf("L = [1;36m%d[0m (# %d)\n", L, M_L);

            for (m = 0; m < M_L; m++)
            {
                mP = m;

                _pdcchCandidates(Yk, mP, N_CCE_k, L);
            }

            printf("\n");
        }
    }


    // UE-specific search space
    {
        int L_ue_specific[]   = {1, 2, 4, 8};
        int M_L_ue_specific[] = {6, 6, 2, 2};
        int A = 39827;
        int D = 65537;
        int Y[10];
        int k;

        Y[0] = (A * RNTI) % D;
        for (k = 1; k < 10; k++)
        {
            Y[k] = (A * Y[k - 1]) % D;
        }

        printf("UE-specific search space:\n");
        Yk = Y[subframe];

        for (i = 0; i < 4; i++)
        {
            L = L_ue_specific[i];
            M_L = M_L_ue_specific[i];
            printf("L = [1;31m%d[0m (# %d)\n", L, M_L);

            for (m = 0; m < M_L; m++)
            {
                mP = m;

                _pdcchCandidates(Yk, mP, N_CCE_k, L);
            }

            printf("\n");
        }
    }


    return 0;
}

