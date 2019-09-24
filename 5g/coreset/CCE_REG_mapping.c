#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG_A 0
#define DEBUG_B 0


int REG[280][4][2];
int CCE[140];


int f(int x, int N_CORESET_REG, int L, int R, int n_shift)
{
    int bundle;
    int C;
    int c;
    int r;

    C = (N_CORESET_REG / (L * R));
    c = ((x / R) % C);
    r = (x % R);
    bundle = (((r * C) + c + n_shift) % (N_CORESET_REG / L));

    #if DEBUG_B
    printf("C %d, c %d, r %d, f(%d) -> %d\n", C, c, r, x, bundle);
    #endif

    return bundle;
}

void do_mapping(
    int interleaved,
    int N_CORESET_RB,
    int N_CORESET_symb,
    int N_CORESET_REG,
    int L,
    int R,
    int n_shift
)
{
    int RB = 0;
    int symb = 0;
    int bundle;
    int i;
    int j;

    #if DEBUG_A
    printf("\n");
    printf("REG   bundle   CCE\n");
    #endif

    for (i=0; i<N_CORESET_REG; i++)
    {
        /* This REG belongs to which CCE:
         *   REG[RB][symb][0] -> REG ID
         *   REG[RB][symb][1] -> bundle ID
         */
        REG[RB][symb][0] = i;
        if ( interleaved )
        {
            bundle = (i / L);
            j = f(bundle, N_CORESET_REG, L, R, n_shift);
            REG[RB][symb][1] = bundle;
            CCE[j] = (bundle / (6 / L));
        }
        else
        {
            bundle = (i / 6);
            j = bundle;
            REG[RB][symb][1] = bundle;
            CCE[j] = bundle;
        }

        #if DEBUG_A
        printf("%2d   %2d -> %2d   %2d\n", i, bundle, j, CCE[j]);
        #endif

        symb++;
        if (symb >= N_CORESET_symb)
        {
            symb = 0;
            RB++;
            if (RB >= N_CORESET_RB)
            {
                ;
            }
        }
    }

    #if DEBUG_A
    printf("\n");
    #endif
}

void show_mapping(int N_CORESET_RB, int N_CORESET_symb, int verbose)
{
    int RB;
    int symb;
    int bundle;
    int CCE_j;

    printf("\n");
    for (RB=(N_CORESET_RB-1); RB>=0; RB--)
    {
        for (symb=0; symb<N_CORESET_symb; symb++)
        {
            bundle = REG[RB][symb][1];
            CCE_j = CCE[ bundle ];
            if (CCE_j & 0x1)
            {
                if ( verbose )
                {
                    printf(" %3d([1;33m%2d[0m:%2d)", REG[RB][symb][0], CCE_j, bundle);
                }
                else
                {
                    printf(" %3d([1;33m%2d[0m)", REG[RB][symb][0], CCE_j);
                }
            }
            else
            {
                if ( verbose )
                {
                    printf(" %3d([1;35m%2d[0m:%2d)", REG[RB][symb][0], CCE_j, bundle);
                }
                else
                {
                    printf(" %3d([1;35m%2d[0m)", REG[RB][symb][0], CCE_j);
                }
            }
        }
        printf("\n");

        if ((RB != 0) && ((RB % 6) == 0))
        {
            printf("\n");
        }
    }
    printf("\n");
}


void help(void)
{
    printf("Usage: CCE_REG_mapping [OPTION]...\n");
    printf("\n");
    printf("  -i                  Interleaved mapping.\n");
    printf("  -b N_CORESET_RB     CORESET RB number (6 ~ 270).\n");
    printf("  -s N_CORESET_symb   CORESET symbol number (1, 2, 3).\n");
    printf("  -l L                REG bundle size (2, 3, 6).\n");
    printf("  -r R                Interleaver size (2, 3, 6).\n");
    printf("  -n n_shift          Shift index (0 ~ 274).\n");
    printf("  -v                  Show REG bundle ID.\n");
    printf("  -h                  Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 7.3.2.2 Control-resource set (CORESET) */
int main(int argc, char *argv[])
{
    int interleaved = 0;
    int N_CORESET_RB = 12;
    int N_CORESET_symb = 3;
    int L = 6;
    int R = 6;
    int n_shift = 0;
    int verbose = 0;

    int N_CORESET_REG;
    int N_REG_bundle;
    int N_CCE;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "ib:s:l:r:n:vh")) != -1)
    {
        switch ( ch )
        {
            case 'i':
                interleaved = 1;
                break;
            case 'b':
                N_CORESET_RB = atoi( optarg );
                if ((N_CORESET_RB < 6) || (N_CORESET_RB > 270))
                {
                    printf("ERR: wrong value of N_CORESET_RB(%d)\n\n", N_CORESET_RB);
                    return -1;
                }
                break;
            case 's':
                N_CORESET_symb = atoi( optarg );
                if ((N_CORESET_symb < 1) || (N_CORESET_symb > 3))
                {
                    printf("ERR: wrong value of N_CORESET_symb(%d)\n\n", N_CORESET_symb);
                    return -1;
                }
                break;
            case 'l':
                L = atoi( optarg );
                if ((L != 2) && (L != 3) && (L != 6))
                {
                    printf("ERR: wrong value of L(%d)\n\n", L);
                    return -1;
                }
                break;
            case 'r':
                R = atoi( optarg );
                if ((R != 2) && (R != 3) && (R != 6))
                {
                    printf("ERR: wrong value of R(%d)\n\n", R);
                    return -1;
                }
                break;
            case 'n':
                n_shift = atoi( optarg );
                if ((n_shift < 0) || (n_shift > 274))
                {
                    printf("ERR: wrong value of n_shift(%d)\n\n", n_shift);
                    return -1;
                }
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    N_CORESET_REG = (N_CORESET_RB * N_CORESET_symb);
    if ((N_CORESET_REG < 6) || (N_CORESET_REG > 810))
    {
        printf("ERR: wrong value of N_CORESET_REG(%d)\n\n", N_CORESET_REG);
        return -1;
    }

    if ( interleaved )
    {
        if (1 == N_CORESET_symb)
        {
            if ((L!= 2) && (L != 6))
            {
                printf("ERR: wrong value of L(%d)\n\n", L);
                return -1;
            }
        }
        if ((N_CORESET_REG % (L * R)) != 0)
        {
            printf("ERR: wrong value of L(%d), R(%d)\n\n", L, R);
            return -1;
        }
    }
    else
    {
        L = 6;
    }
    N_REG_bundle = (N_CORESET_REG / L);
    N_CCE = (N_CORESET_REG / 6);

    printf("N_CORESET_REG     = %d\n", N_CORESET_REG);
    printf("N_CORESET_RB      = %d\n", N_CORESET_RB);
    printf("N_CORESET_symb    = %d\n", N_CORESET_symb);
    printf("L                 = %d\n", L);
    printf("R                 = %d\n", R);
    printf("n_shift           = %d\n", n_shift);
    printf("REG bundle number = %d\n", N_REG_bundle);
    printf("CCE number        = %d\n", N_CCE);

    do_mapping(
        interleaved,
        N_CORESET_RB,
        N_CORESET_symb,
        N_CORESET_REG,
        L,
        R,
        n_shift
    );
    show_mapping(N_CORESET_RB, N_CORESET_symb, verbose);


    return 0;
}

