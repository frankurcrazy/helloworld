#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "npdcch_common.c"


char *G2str(int G)
{
    char *str = NULL;

    if (3 == G)
    {
        str = _GStr[0];
    }
    else if (4 == G)
    {
        str = _GStr[1];
    }
    else if (8 == G)
    {
        str = _GStr[2];
    }
    else if (16 == G)
    {
        str = _GStr[3];
    }
    else if (32 == G)
    {
        str = _GStr[4];
    }
    else if (64 == G)
    {
        str = _GStr[5];
    }
    else if (96 == G)
    {
        str = _GStr[6];
    }
    else if (128 == G)
    {
        str = _GStr[7];
    }

    return str;
}

char *alpha2str(int alpha)
{
    return _alphaStr[alpha & 0x3];
}


int Rmax2R(int type, int Rmax, int repNum)
{
    int R = 0;

    if (0 == type)
    {
        int div[4] = { 8, 4, 2, 1 };

        /* UE-specific search space (Table 16.6-1) */
        switch ( Rmax )
        {
            case 1:
                R = 1;
                break;
            case 2:
                R = ((repNum & 0x1) + 1);
                break;
            case 4:
                if ((repNum & 0x3) > 0)
                {
                    R = ((repNum & 0x3) << 1);
                }
                else
                {
                    R = 1;
                }
                break;
            case 8:
            case 16:
            case 32:
            case 64:
            case 128:
            case 256:
            case 512:
            case 1024:
            case 2048:
                R = (Rmax / div[repNum & 0x3]);
                break;
            default:
                ;
        }
    }
    else if (1 == type)
    {
        int Rtable[12][8] = {
            { 1, 0,  0,   0,   0,   0,    0,    0 },
            { 1, 2,  0,   0,   0,   0,    0,    0 },
            { 1, 2,  4,   0,   0,   0,    0,    0 },
            { 1, 2,  4,   8,   0,   0,    0,    0 },
            { 1, 2,  4,   8,  16,   0,    0,    0 },
            { 1, 2,  4,   8,  16,  32,    0,    0 },
            { 1, 2,  4,   8,  16,  32,   64,    0 },
            { 1, 2,  4,   8,  16,  32,   64,  128 },
            { 1, 4,  8,  16,  32,  64,  128,  256 },
            { 1, 4, 16,  32,  64, 128,  256,  512 },
            { 1, 8, 32,  64, 128, 256,  512, 1024 },
            { 1, 8, 64, 128, 256, 512, 1024, 2048 }
        };

        /* Type1 Common search space */
        switch ( Rmax )
        {
            case 1:
                R = Rtable[0][repNum & 0x7];
                break;
            case 2:
                R = Rtable[1][repNum & 0x7];
                break;
            case 4:
                R = Rtable[2][repNum & 0x7];
                break;
            case 8:
                R = Rtable[3][repNum & 0x7];
                break;
            case 16:
                R = Rtable[4][repNum & 0x7];
                break;
            case 32:
                R = Rtable[5][repNum & 0x7];
                break;
            case 64:
                R = Rtable[6][repNum & 0x7];
                break;
            case 128:
                R = Rtable[7][repNum & 0x7];
                break;
            case 256:
                R = Rtable[8][repNum & 0x7];
                break;
            case 512:
                R = Rtable[9][repNum & 0x7];
                break;
            case 1024:
                R = Rtable[10][repNum & 0x7];
                break;
            case 2048:
                R = Rtable[11][repNum & 0x7];
                break;
            default:
                ;
        }
    }
    else if (2 == type)
    {
        int div[4] = { 8, 4, 2, 1 };

        /* Type2 Common search space */
        switch ( Rmax )
        {
            case 1:
                R = 1;
                break;
            case 2:
                R = ((repNum & 0x1) + 1);
                break;
            case 4:
                if ((repNum & 0x3) > 0)
                {
                    R = ((repNum & 0x3) << 1);
                }
                else
                {
                    R = 1;
                }
                break;
            case 8:
            case 16:
            case 32:
            case 64:
            case 128:
            case 256:
            case 512:
            case 1024:
            case 2048:
                R = (Rmax / div[repNum & 0x3]);
                break;
            default:
                ;
        }
    }

    return R;
}


void ueSpecific(int Rmax, int G, int alpha, int repNum, int verbose)
{
    int T;
    int alphaT;
    int k0;
    int k;
    int count;

    int R;
    int U;
    int u;
    int b;


    k0 = npdcch_k0(Rmax, G, alpha, &T, &alphaT);
    if (k0 >= 0)
    {
        count = 0;

        printf(
            "Rmax( %d ), G( %s ), alpha( %s ), T( %d ), alphaT( %d )\n",
            Rmax,
            G2str(G),
            alpha2str(alpha),
            T,
            alphaT
        );
        printf("\n");
        printf("[1;35mk0:[0m\n");
        printf(" %d ==> +%d\n", alphaT, T);
        for (k=0; k<10240; k++)
        {
            if ((k % T) == alphaT)
            {
                if ( verbose )
                {
                    if (((count % 10) == 0) && (count != 0))
                    {
                        printf("\n");
                    }
                    printf(" %5d", k);
                }
                count++;
            }
        }
        if ( verbose )
        {
            printf("\n");
        }
        printf(" Number of subframe_k0 = %d\n", count);
        printf("\n");


        R = Rmax2R(0, Rmax, repNum);
        if (R > 0)
        {
            U = (Rmax / R);

            printf("R( %d ), DCI repetition( %d )\n", R, repNum);
            printf("\n");
            printf("[1;33mb:[0m\n");
            for (u=0; u<U; u++)
            {
                b = (u * R);
                printf("  %d", b);
            }
            printf("\n");
            printf(" Number of subframe_kb = %d\n", U);
            printf("\n");
        }
    }
}

void type1Common(int Rmax, int G, int alpha, int repNum, int verbose)
{

}

void type2Common(int Rmax, int G, int alpha, int repNum, int verbose)
{
    int T;
    int alphaT;
    int k0;
    int k;
    int count;

    int R;
    int U;
    int u;
    int b;


    k0 = npdcch_k0(Rmax, G, alpha, &T, &alphaT);
    if (k0 >= 0)
    {
        count = 0;

        printf(
            "Rmax( %d ), G( %s ), alpha( %s ), T( %d ), alphaT( %d )\n",
            Rmax,
            G2str(G),
            alpha2str(alpha),
            T,
            alphaT
        );
        printf("\n");
        printf("[1;35mk0:[0m\n");
        printf(" + %d\n", (T + alphaT));
        for (k=0; k<10240; k++)
        {
            if ((k % T) == alphaT)
            {
                if ( verbose )
                {
                    if (((count % 10) == 0) && (count != 0))
                    {
                        printf("\n");
                    }
                    printf(" %5d", k);
                }
                count++;
            }
        }
        if ( verbose )
        {
            printf("\n");
        }
        printf(" Number of subframe_k0 = %d\n", count);
        printf("\n");


        R = Rmax2R(2, Rmax, repNum);
        if (R > 0)
        {
            U = (Rmax / R);

            printf("R( %d ), DCI repetition( %d )\n", R, repNum);
            printf("\n");
            printf("[1;33mb:[0m\n");
            for (u=0; u<U; u++)
            {
                b = (u * R);
                printf("  %d", b);
            }
            printf("\n");
            printf(" Number of subframe_kb = %d\n", U);
            printf("\n");
        }
    }
}

void help(void)
{
    printf("Usage: npdcch_subframe [OPTION]...\n");
    printf("\n");
    printf("  -c type    Search space type.\n");
    printf("               0: USS\n");
    printf("               1: CSS type 1\n");
    printf("               2: CSS type 2\n");
    printf("  -r Rmax    Rmax (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048).\n");
    printf("  -g G       G (3, 4, 8, 16, 32, 64, 96, 128).\n");
    printf("  -a alpha   Alpha offset (0, 1, 2, 3).\n");
    printf("  -n repNum  DCI subframe repetition number (0, 1, 2, 3).\n");
    printf("  -v         Verbose log.\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.213 Section 16.6 Narrowband physical downlink control channel related procedures
*/
int main(int argc, char *argv[])
{
    int type = 0;
    int Rmax = 16;
    int G = 4;
    int alpha = 3;
    int repNum = 0;
    int verbose = 0;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:r:g:a:n:vh")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                type = atoi( optarg );
                break;
            case 'r':
                Rmax = atoi( optarg );
                break;
            case 'g':
                G = atoi( optarg );
                break;
            case 'a':
                alpha = atoi( optarg );
                break;
            case 'n':
                repNum = atoi( optarg );
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

    if (0 == type)
    {
        printf("[ UE-specific Search Space ]\n\n");
        ueSpecific(Rmax, G, alpha, repNum, verbose);
    }
    else if (1 == type)
    {
        printf("[ Type-1 Common Search Space ]\n\n");
        type1Common(Rmax, G, alpha, repNum, verbose);
    }
    else if (2 == type)
    {
        printf("[ Type-2 Common Search Space ]\n\n");
        type2Common(Rmax, G, alpha, repNum, verbose);
    }
    else
    {
        printf("Unknown search space type %d\n\n", type);
    }

    return 0;
}

