#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int factorial(int n)
{
    if (0 == n)
    {
        return 1;
    }
    else
    {
        return (n * factorial(n - 1));
    }
}

int combination(int x, int y)
{
    int c = 0;

    if (x >= y)
    {
        c = factorial( x ) / (factorial( y ) * factorial(x - y));
    }

    return c;
}

int r_encode(int N_DL_RB, int N_Xp_RB, int k[])
{
    int r = 0;
    int c;
    int i;

    //printf("comb: ");
    for (i=0; i<N_Xp_RB; i++)
    {
        c = combination((N_DL_RB - k[i]), (N_Xp_RB - i));
        //printf(" %2d", c);
        r += c;
    }
    //printf("\n");

    return r;
}


int k2[15][2] = {
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 1, 5 },
    { 1, 6 },
    { 2, 3 },
    { 2, 4 },
    { 2, 5 },
    { 2, 6 },
    { 3, 4 },
    { 3, 5 },
    { 3, 6 },
    { 4, 5 },
    { 4, 6 },
    { 5, 6 }
};

int k4[15][4] = {
    { 1, 2, 3, 4 },
    { 1, 2, 3, 5 },
    { 1, 2, 3, 6 },
    { 1, 2, 4, 5 },
    { 1, 2, 4, 6 },
    { 1, 2, 5, 6 },
    { 1, 3, 4, 5 },
    { 1, 3, 4, 6 },
    { 1, 3, 5, 6 },
    { 1, 4, 5, 6 },
    { 2, 3, 4, 5 },
    { 2, 3, 4, 6 },
    { 2, 3, 5, 6 },
    { 2, 4, 5, 6 },
    { 3, 4, 5, 6 }
};

int main(int argc, char *argv[])
{
    int N_DL_RB = 6;
    int N_Xp_RB;
    int r;
    int n;


    N_Xp_RB = 2;
    printf("N_Xp_RB = %d\n", N_Xp_RB);
    for (n=0; n<15; n++)
    {
        r = r_encode(N_DL_RB, N_Xp_RB, k2[n]);
        printf("%3d ... PRB[%d, %d]\n", r, k2[n][0], k2[n][1]);

    }
    printf("\n");

    N_Xp_RB = 4;
    printf("N_Xp_RB = %d\n", N_Xp_RB);
    for (n=0; n<15; n++)
    {
        r = r_encode(N_DL_RB, N_Xp_RB, k4[n]);
        printf("%3d ... PRB[%d, %d, %d, %d]\n", r, k4[n][0], k4[n][1], k4[n][2], k4[n][3]);

    }
    printf("\n");


    return 0;
}

