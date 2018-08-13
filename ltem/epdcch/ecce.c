#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


static void _mpdcchCandidates(int Y, int m, int M_L, int N_ECCE, int L)
{
    int ecceIndex;
    int i;

    printf(" {");

    for (i = 0; i < L; i++)
    {
        ecceIndex = L * ((Y + DIV_FLOOR((m * N_ECCE), (L * M_L))) % DIV_FLOOR(N_ECCE, L)) + i;
        printf(" %2d", ecceIndex);
    }

    printf(" }\n");
}



int main(int argc, char *argv[])
{
    int N_Xp_RB = 2;
    int N_ECCE_EREG = 4;
    int N_ECCE;
    int subframe = 0;
    int RNTI = 12;

    int M_L;
    int Yk;
    int m;
    int L;
    int i;


    N_ECCE = (N_Xp_RB * DIV_FLOOR(16, N_ECCE_EREG));
    printf("N_ECCE = %d\n", N_ECCE);
    printf("\n");


#if 0
    // Common search space
    {
        int L_common[]   = {4, 8};
        int M_L_common[] = {4, 2};

        printf("Common search space:\n");
        Y = 0;

        for (i = 0; i < 2; i++)
        {
            L = L_common[i];
            printf("L = %d\n", L);

            for (m = 0; m < M_L_common[i]; m++)
            {
                _mpdcchCandidates(Y, m, M_L_common[i], N_ECCE, L);
            }

            printf("\n");
        }
    }
#endif


    // UE-specific search space
    {
        int L_ue_specific[]   = {2, 4, 8, 16};
        int M_L_ue_specific[] = {4, 2, 1, 0};  // N_Xp_RB = 2
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
            printf("L = %d\n", L);

            for (m = 0; m < M_L_ue_specific[i]; m++)
            {
                M_L = M_L_ue_specific[i];

                _mpdcchCandidates(Yk, m, M_L, N_ECCE, L);
            }

            printf("\n");
        }
    }



    return 0;
}
