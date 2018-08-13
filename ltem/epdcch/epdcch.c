#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int max(int a, int b)
{
    return ((a >= b) ? a : b);
}

/* 36.211 - 6.8A.1 EPDCCH formats */
int main(int argc, char *argv[])
{
    int N_Xm_RB = 2;
    int N_ECCE_EREG = 4;
    int N_ECCE_m_i;
    int N_RB_ECCE;
    int EREG;
    int PRB;
    int localized = 1;
    int n;
    int j;


    if (argc > 3)
    {
        N_ECCE_EREG = atoi( argv[1] );
        N_Xm_RB = atoi( argv[2] );
        localized = atoi( argv[3] );
    }
    else if (argc > 2)
    {
        N_ECCE_EREG = atoi( argv[1] );
        N_Xm_RB = atoi( argv[2] );
    }
    else if (argc > 1)
    {
        N_ECCE_EREG = atoi( argv[1] );
    }
    else
    {
        printf("[1] N_ECCE_EREG = %d (4 or 8)\n", N_ECCE_EREG);
        printf("[2] N_Xm_RB = %d (2 or 4)\n", N_Xm_RB);
        printf("[3] localized = %d\n", localized);
        printf("\n");
    }

    if ((N_ECCE_EREG != 4) && (N_ECCE_EREG != 8))
    {
        printf("ERR: wrong N_ECCE_EREG\n");
        return 0;
    }

    if ((N_Xm_RB != 2) && (N_Xm_RB != 8))
    {
        printf("ERR: wrong N_Xm_RB\n");
        return 0;
    }


    N_RB_ECCE = (16 / N_ECCE_EREG);
    N_ECCE_m_i = (N_RB_ECCE * N_Xm_RB);

    printf("==============================================\n");
    printf("ECCE | PRB | EREG(s)\n");
    printf("==============================================\n");
    for (n=0; n<N_ECCE_m_i; n++)
    {
        if ( localized )
        {
            PRB = DIV_FLOOR(n, N_RB_ECCE);
            printf(" %2d  | %2d  |", n, PRB);

            for (j=0; j<N_ECCE_EREG; j++)
            {
                EREG = (n % N_RB_ECCE) + (j * N_RB_ECCE);
                printf(" %2d ", EREG);
            }
            printf("\n");
        }
        else
        {
            for (j=0; j<N_ECCE_EREG; j++)
            {
                PRB = ((n + (j * max(1, N_Xm_RB / N_ECCE_EREG))) % N_Xm_RB);

                EREG = DIV_FLOOR(n, N_Xm_RB) + (j * N_RB_ECCE);

                printf(" %2d  | %2d  |  %2d\n", n, PRB, EREG);
            }
            if (n < (N_ECCE_m_i - 1)) printf("     |     |\n");
        }

        if ((n < (N_ECCE_m_i - 1)) && ((n % N_RB_ECCE) == (N_RB_ECCE - 1)))
        {
            printf("-----+-----+----------------------------------\n");
        }
    }
    printf("==============================================\n");
    printf("N_Xm_RB = %d\n", N_Xm_RB);
    printf("N_RB_ECCE = %d\n", N_RB_ECCE);
    printf("N_ECCE_EREG = %d\n", N_ECCE_EREG);
    printf("\n");


    return 0;
}
