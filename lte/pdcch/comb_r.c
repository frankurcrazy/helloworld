#include <stdio.h>


int s[4] = {
    13,
    10,
    6,
    4
};


/*
* f(n) = n * (n - 1) * (n - 2) * ... * 1
*/
unsigned long long factorial(int n)
{
    unsigned long long f = 1;
    int i;

    for (i=0; i<n; i++)
    {
        f *= (i + 1);
    }

    return f;
}

/*
*           n           n!
* C(n,r) = ( ) = ---------------
*           r     r! x (n - r)!
*/
int combination(int n, int r)
{
    int c;

    c = (factorial(n) / (factorial(r) * factorial(n - r)));

    return c;
}

/*
 * R10 36.213 Section 8.1.2 Uplink Resource allocation type 1
 */
int main(void)
{
    int comb_r = 0;

    // Begin of encoding
    {
        int r[4];
        int i;
        int j;

        printf("Encoding:\n");

        for (i=0, j=4; i<4; i++, j--)
        {
            printf("s[%d] = %d\n", i, s[i]);

            r[i] = combination(s[i], j);

            comb_r += r[i];
        }

        printf(
            "r = %d + %d + %d + %d = %d\n",
            r[0],
            r[1],
            r[2],
            r[3],
            comb_r
        );
        printf("\n");
    }
    // End   of encoding


    // Begin of decoding
    {
        int N = comb_r;
        int r[4];
        int s1;
        int s2;
        int c1;
        int c2;
        int ck;
        int i;
        int j;

        printf("Decoding:\n");
        printf("r = %d\n", N);

        for (i=0, j=4; i<4; i++, j--)
        {
            //printf("[%d] N = %d\n", i, N);
            s1 = (j - 1);
            s2 = 0;

            do
            {
                c1 = combination(s1, j);
                ck = c2;
                c2 = c1;
                r[i] = s2;
                s2 = s1;
                s1++;
            }
            while (c1 <= N);

            //printf("[%d] c = %d\n", i, ck);
            N -= ck;
        }

        printf("s[0] = %d\n", r[0]);
        printf("s[1] = %d\n", r[1]);
        printf("s[2] = %d\n", r[2]);
        printf("s[3] = %d\n", r[3]);
        printf("\n");
    }
    // End   of decoding

    return 0;
}
