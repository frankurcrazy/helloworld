#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int mapping(int N_UL_RB, int m, int N_PUCCH_abs, int N_ch_UL_NB, int i)
{
    int nPRB;
    int mP;
    int j;


    j = DIV_FLOOR(i, N_ch_UL_NB);
    //printf("j %d <= i %d\n", j , i);

    if (0 == (j % 2))
    {
        mP = m;
    }
    else
    {
        if (0 == (m % 2))
        {
            mP = m + 1;
        }
        else
        {
            mP = m - 1;
        }
    }

    if (0 == (mP % 2))
    {
        nPRB = (mP / 2);
    }
    else
    {
        nPRB = N_UL_RB - 1 - DIV_FLOOR(mP, 2);
    }

    return nPRB;
}

/*
 * R13 36.211 Section 5.4.3	Mapping to physical resources
 */
int main(int argc, char *argv[])
{
    int N_UL_RB = 100;
    int N_PUCCH_rep = 16;
    int N_PUCCH_abs = N_PUCCH_rep;
    int N_ch_UL_NB = 2;
    int frame = 8;
    int subframe = 1;
    int m = 3;
    int nPRB;
    int i;
    int n;

    if (argc > 3)
    {
        m = atoi( argv[1] );
        subframe = atoi( argv[2] );
        frame = atoi( argv[3] );
    }
    if (argc > 2)
    {
        m = atoi( argv[1] );
        subframe = atoi( argv[2] );
    }
    else if (argc > 1)
    {
        m = atoi( argv[1] );
    }
    else
    {
        printf("Usage: nPRB <m> <subframe_number> <frame_number>\n");
        printf("\n");
    }

    printf("subframe  RB\n");

    i = (10 * frame) + subframe;
    for (n=0; n<N_PUCCH_abs; n++)
    {
        nPRB = mapping(N_UL_RB, m, N_PUCCH_abs, N_ch_UL_NB, i);
        printf("    %4d  %2d\n", i, nPRB);
        i++;
    }

    printf("\n");

    return 0;
}

