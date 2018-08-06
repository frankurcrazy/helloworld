#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void lte_prs_generate(unsigned int c_init, int len, unsigned char *buf)
{
    unsigned char *c = buf;
    unsigned char *x1, *x2;
    int Nc = 1600;
    int n;

    x1 = malloc( Nc + len + 31 );
    if ( !x1 )
    {
        printf("%s: fail to allocate memory for x1\n", __func__);
        return;
    }

    x2 = malloc( Nc + len + 31 );
    if ( !x2 )
    {
        printf("%s: fail to allocate memory for x2\n", __func__);
        free( x1 );
        return;
    }

    for (n=0; n<31; n++)
    {
        if (0 == n) x1[0] = 1;
        else        x1[n] = 0;
        x2[n] = (c_init >> n) & 0x1;
    }

    for (n=0; n<(Nc + len); n++)
    {
        x1[n + 31] = (x1[n + 3] + x1[n]) & 0x1;
        x2[n + 31] = (x2[n + 3] + x2[n + 2] + x2[n + 1] + x2[n]) & 0x1;
    }

    for (n=0; n<len; n++)
    {
        c[n] = (x1[n + Nc] + x2[n + Nc]) & 0x1;
    }

    free( x1 );
    free( x2 );
}


void help(void)
{
    printf("Usage: pseudo_random_sequence [OPTION]...\n");
    printf("\n");
    printf("  -c c_init   c_init value\n");
    printf("  -l length   Sequence length\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/*
*  R10 36.211 Section 7.2 Pseudo-random sequence generation
*/
int main(int argc, char *argv[])
{
    unsigned char seq[1024];
    int c_init = 0;
    int len = 16;
    int ch;
    int i;

    opterr = 0;
    while ((ch=getopt(argc, argv, "c:l:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                c_init = atoi( optarg );
                break;
            case 'l':
                len = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    lte_prs_generate(c_init, len, seq);

    printf("c_init = %d\n", c_init);
    printf("length = %d\n", len);
    printf("\n");
    for (i=0; i<len; i++)
    {
        if (((i % 16) == 0) && (i != 0)) printf("\n");
        printf(" %d", seq[i]);
    }
    printf("\n");
    printf("\n");

    return 0;
}

