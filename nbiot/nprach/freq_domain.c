#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define DIV_FLOOR(X, Y)  ( (X) / (Y) )

#define NUM_OF_RA_SC  (12)


unsigned char cn[1280];


void lte_prs_generate(unsigned int c_init, unsigned char *buf, int len)
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


unsigned int sum_cn(int t)
{
    unsigned int sum = 0;
    int n;
    int i;

    for (i=1; i<10; i++)
    {
        n = ((10 * t) + i);
        sum += (cn[n] << (i - 1));
    }

    return sum;
}

void calc_f(int *f, int len)
{
    int N_RA_sc = NUM_OF_RA_SC;
    int t;

    f[0] = (((sum_cn( 0 ) % (N_RA_sc - 1)) + 1) % N_RA_sc);

    for (t=1; t<len; t++)
    {
        f[t] = ((f[t - 1] + (sum_cn( t ) % (N_RA_sc - 1)) + 1) % N_RA_sc);
    }
}


int calc_n_RA_sc(
    int *n_RA_sc,
    int  N_NPRACH_rep,
    int  N_NPRACH_scoffset,
    int  N_Ncell_ID,
    int  n_init
)
{
    int N_RA_sc = NUM_OF_RA_SC;
    int n_start;
    int t;
    int i;

    int _n_RA_sc[512];
    int f[128];

    calc_f(f, N_NPRACH_rep);

    n_start = (N_NPRACH_scoffset + (DIV_FLOOR(n_init, N_RA_sc) * N_RA_sc));

    /* 4 symbol groups per on transmission */
    for (i=0; i<(4 * N_NPRACH_rep); i++)
    {
        if (0 == i)
        {
            _n_RA_sc[0] = (n_init % N_RA_sc);
        }
        else
        {
            switch (i % 4)
            {
                default:
                case 0:
                    t = (i >> 2);
                    _n_RA_sc[i] = ((_n_RA_sc[0] + f[t]) % N_RA_sc);
                    break;
                case 1:
                case 3:
                    if ((_n_RA_sc[i - 1] % 2) == 0)
                    {
                        _n_RA_sc[i] = (_n_RA_sc[i - 1] + 1);
                    }
                    else
                    {
                        _n_RA_sc[i] = (_n_RA_sc[i - 1] - 1);
                    }
                    break;
                case 2:
                    if (_n_RA_sc[i - 1] < 6)
                    {
                        _n_RA_sc[i] = (_n_RA_sc[i - 1] + 6);
                    }
                    else
                    {
                        _n_RA_sc[i] = (_n_RA_sc[i - 1] - 6);
                    }
                    break;
            }
        }

        n_RA_sc[i] = (n_start + _n_RA_sc[i]);
    }

    return (4 * N_NPRACH_rep);
}


void help(void)
{
    printf("Usage: freq_loc [OPTION]...\n");
    printf("\n");
    printf("  -o N_NPRACH_scoffset  (0, 12, 24, 36, 2, 18, 34).\n");
    printf("  -s N_NPRACH_sc        (12, 24, 36, 48).\n");
    printf("  -r N_NPRACH_rep       (1, 2, 4, 8, 16, 32, 64, 128).\n");
    printf("  -c N_Ncell_ID         (0 .. 503).\n");
    printf("  -i n_init             (0 .. (N_NPRACH_sc - 1)).\n");
    printf("  -d                    Enable the debug log.\n");
    printf("  -h                    Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.211 Section 10.1.6 Narrowband physical random access channel
*/
int main(int argc, char *argv[])
{
    int   N_NPRACH_scoffset = 12; /* 0, 12, 24, 36, 2, 18, 34 subcarriers */
    int   N_NPRACH_sc = 12; /* 12, 24, 36, 48 subcarriers */
    int   N_NPRACH_rep = 4; /* 1, 2, 4, 8, 16, 32, 64, 128 */
    //int   N_NPRACH_start = 16; /* 8, 16, 32, 64, 128, 256, 512, 1024 ms */
    int   N_Ncell_ID = 0;

    int   debug = 0;
    int   n_init = 9;
    int   n_RA_sc[512];
    int   num;
    int   i;
    int   ch;


    //if (1 == argc)
    //{
    //    help();
    //    return -1;
    //}

    opterr = 0;
    while ((ch=getopt(argc, argv, "o:s:r:c:i:dh")) != -1)
    {
        switch ( ch )
        {
            case 'o':
                N_NPRACH_scoffset = atoi( optarg );
                break;
            case 's':
                N_NPRACH_sc = atoi( optarg );
                break;
            case 'r':
                N_NPRACH_rep = atoi( optarg );
                break;
            case 'c':
                N_Ncell_ID = atoi( optarg );
                break;
            case 'i':
                n_init = atoi( optarg );
                break;
            case 'd':
                debug = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ((N_NPRACH_scoffset + N_NPRACH_sc) > 48)
    {
        printf("incorrect subcarrier range %d\n", (N_NPRACH_scoffset + N_NPRACH_sc));
        exit(0);
    }

    if (n_init > (N_NPRACH_sc - 1))
    {
        printf("incorrect n_init %d\n", n_init);
        exit(0);
    }

    printf("N_NPRACH_scoffset = %d\n", N_NPRACH_scoffset);
    printf("N_NPRACH_sc       = %d\n", N_NPRACH_sc);
    printf("N_NPRACH_rep      = %d\n", N_NPRACH_rep);
    printf("N_Ncell_ID        = %d\n", N_Ncell_ID);
    printf("n_init            = %d\n", n_init);
    printf("\n");


    lte_prs_generate(N_Ncell_ID, cn, sizeof( cn ));


    num = calc_n_RA_sc(
              n_RA_sc,
              N_NPRACH_rep,
              N_NPRACH_scoffset,
              N_Ncell_ID,
              n_init
          );

    #if 1
    if ( debug )
    {
        printf("Subcarrier location of [1;35m%d[0m symbol group(s):\n", num);
        for (i=0; i<num; i++)
        {
            if ((i % 4) == 0) printf(" ");
            printf("%d ", n_RA_sc[i]);
        }
        printf("\n\n");
    }
    #endif

    {
        int N_sc;
        int sc;
        int group;
        int rep;

        printf("[1;33mSC[0m\n");
        for (sc=N_NPRACH_sc; sc>0; sc--)
        {
            N_sc = (N_NPRACH_scoffset + sc - 1);
            printf("%2d  ", N_sc);
            for (rep=0; rep<N_NPRACH_rep; rep++)
            {
                for (group=0; group<4; group++)
                {
                    i = ((rep * 4) + group);

                    if (n_RA_sc[i] == N_sc)
                        printf("[[1;36m%2d[0m]", n_RA_sc[i]);
                    else
                        printf("[  ]");
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}

