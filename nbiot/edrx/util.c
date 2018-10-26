#include <stdio.h>

int get_bits(unsigned long long val64)
{
    int len = 0;
    int i;

    for (i=64; i>0; i--)
    {
        if ((0x1LL << (i-1)) & val64)
        {
            len = i;
            break;
        }
    }

    return len;
}

void print_bits(unsigned long long val64, char *pStr)
{
    int len;
    int i;
    int j;

    len = get_bits( val64 );
    for (i=len, j=0; i>0; i--)
    {
        pStr[j++] = (((0x1LL << (i-1)) & val64) ? '1' : '0');
    }
    pStr[j] = '\0';
}

int cmp_val64(unsigned long long A, unsigned long long B)
{
    unsigned long long mask;
    int i;

    for (i=64; i>0; i--)
    {
        mask = (0x1LL << (i-1));
        if ((mask & A) && !(mask & B))
        {
            /* A > B */;
            return 1;
        }
        else if (!(mask & A) && (mask & B))
        {
            /* A < B */;
            return -1;
        }
        else
        {
            /* A == B */;
            ;
        }
    }

    return 0;
}

unsigned long long bin_div64(
    unsigned long long  A,
    unsigned long long  B,
    unsigned long long *R
)
{
    unsigned long long Q = 0;
    unsigned long long rem;
    unsigned long long sub;
    unsigned int i;
    char string[2][80];
    int bits[2];
    int shift;


    if (-1 == cmp_val64(A, B))
    {
        if ( R ) *R = A;
        return 0;
    }

    bits[0] = get_bits( A );
    bits[1] = get_bits( B );

    i = 1;
    rem = A;
    sub = B;
    while (bits[0] >= bits[1])
    {
        print_bits(rem, &(string[0][0]));
        print_bits(sub, &(string[1][0]));
        //printf("%u) %s / %s\n", i++, string[0], string[1]);

        shift = (bits[0] - bits[1]);
        Q |= (0x1LL << shift);
        rem = (rem ^ (sub << shift));

        bits[0] = get_bits( rem );
    }

    if ( R ) *R = rem;

    return Q;
}

unsigned long long calc_fcs(
    unsigned long long Y1,
    unsigned long long Y2
)
{
    unsigned long long fcs = 0;
    unsigned long long mask;
    int len;


    len  = get_bits(Y1 ^ Y2);
    mask = ((0x1LL << len) - 1);

    fcs  = ~(Y1 ^ Y2);
    fcs &= mask;

    return fcs;
}

