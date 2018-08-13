#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


/* 36.211 - 6.10.3A Demodulation reference signals associated with EPDCCH or MPDCCH */
int main(int argc, char *argv[])
{
    int lP[2][2] = { {0, 1}, {2, 3} };
    int N_RB_sc = 12;
    int nPRB = 0;
    int mP;
    int kP;
    int k;
    int l;
    int i;


    printf(" slot[0]  slot[1]\n");
    for (mP=0; mP<3; mP++)
    {
        for (kP=0; kP<2; kP++)
        {
            k = ((5 * mP) + (N_RB_sc * nPRB) + kP);
            //printf("k = %d\n", k);

            for (i=0; i<2; i++)
            {
                /* slot[0] */
                l = ((lP[0][i] % 2) + 5);
                printf(" (%2d, %2d)", k, l);

                /* slot[1] */
                l = ((lP[1][i] % 2) + 5);
                printf(" (%2d, %2d)\n", k, l);
            }
        }
    }


    return 0;
}
