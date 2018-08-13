#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int format1x(int n_1_PUCCH, int N_1_cs, int N_2_RB, int delta_PUCCH_shift)
{
    int N_RB_sc = 12;
    int c = 3; // normal CP
    int N1;
    int m;



    N1 = ((c * N_1_cs) / delta_PUCCH_shift);

    if (n_1_PUCCH < N1)
    {
        m = N_2_RB;
    }
    else
    {
        m = DIV_FLOOR((n_1_PUCCH - N1), ((c * N_RB_sc) / delta_PUCCH_shift)) + N_2_RB + DIV_CEIL(N_1_cs, 8);
    }

    return m;
}

/*
 * R13 36.211 Section 5.4.3	Mapping to physical resources
 */
int main(int argc, char *argv[])
{
    int n_1_PUCCH = 36;
    int N_1_cs = 0;
    int N_2_RB = 2;
    int delta_PUCCH_shift = 1;
    int m;

    if (argc > 5)
    {
        n_1_PUCCH = atoi( argv[1] );
        N_1_cs = atoi( argv[2] );
        N_2_RB = atoi( argv[3] );
        delta_PUCCH_shift = atoi( argv[4] );
    }
    else if (argc > 4)
    {
        n_1_PUCCH = atoi( argv[1] );
        N_1_cs = atoi( argv[2] );
        N_2_RB = atoi( argv[3] );
    }
    else if (argc > 3)
    {
        n_1_PUCCH = atoi( argv[1] );
        N_1_cs = atoi( argv[2] );
    }
    else if (argc > 2)
    {
        n_1_PUCCH = atoi( argv[1] );
    }
    else
    {
        printf("Usage: calc_m <n_1_PUCCH> <N_1_cs> <N_2_RB> <delta_PUCCH_shift>\n");
        printf("\n");
    }

    printf("n_1_PUCCH = %d\n", n_1_PUCCH);
    printf("N_1_cs = %d\n", N_1_cs);
    printf("N_2_RB = %d\n", N_2_RB);
    printf("delta_PUCCH_shift = %d\n", delta_PUCCH_shift);
    printf("\n");

    m = format1x(n_1_PUCCH, N_1_cs, N_2_RB, delta_PUCCH_shift);


    printf("m = %d\n", m);
    printf("\n");

    return 0;
}

