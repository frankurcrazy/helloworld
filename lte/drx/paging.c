#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define CEIL(x, y)   (((x) + ((y) - 1)) / (y))
#define FLOOR(x, y)  ((x) / (y))


/* 36.304 7.2 Subframe Patterns */
int _sfPattern[5][4] = {
    { -1, -1, -1, -1  },
    {  9, -1, -1, -1  },  //1
    {  4,  9, -1, -1  },  //2
    { -1, -1, -1, -1  },
    {  0,  4,  5,  9  }   //4
};

char *_nBStr[13] = {
    "4T", "2T", "T", "T/2", "T/4", "T/8", "T/16", "T/32",
    "T/64", "T/128", "T/256", "T/512", "T/1024"
};


unsigned long long imsiStrToInt(char *pStr)
{
    unsigned long long IMSI = 0;
    unsigned long long table[] = {
        1LL,
        10LL,
        100LL,
        1000LL,
        10000LL,
        100000LL,
        1000000LL,
        10000000LL,
        100000000LL,
        1000000000LL,
        10000000000LL,
        100000000000LL,
        1000000000000LL,
        10000000000000LL,
        100000000000000LL
    };
    int len;
    int i;

    len = strlen( pStr );
    if (len > 15) len = 15;

    IMSI = 0;
    for (i=len; i>0; i--)
    {
        IMSI += ((pStr[i-1] - '0') * table[len-i]);
    }

    return IMSI;
}

void show_PF_PO(int UE_ID, int T, int N, int Ns, int i_s)
{
    int SFN;
    int PF;  /* Paging Frame */
    int PO;  /* Paging Occasion */
    int j;


    printf("------+-------------\n");
    printf("Frame | Subframe(s)\n");
    printf("------+-------------\n");
    for (SFN=0; SFN<1024; SFN++)
    {
        if ((SFN % T) == ((T / N) * (UE_ID % N)))
        {
            PF = SFN;
            PO = _sfPattern[Ns][i_s];

            printf("[1;33m%4d[0m  | ", SFN);
            for (j=0; j<4; j++)
            {
                if (_sfPattern[Ns][j] >= 0)
                {
                    if (i_s == j)
                    {
                        printf(" [1;35m%d[0m", _sfPattern[Ns][j]);
                    }
                    else
                    {
                        printf(" %d", _sfPattern[Ns][j]);
                    }
                }
            }
            printf("\n");
        }
    }
    printf("------+-------------\n");
    printf("\n");
}

void help(void)
{
    printf("Usage: paging [OPTION]...\n");
    printf("\n");
    printf("  -i IMSI  IMSI string (15 characters).\n");
    printf("  -t T     DRX cycle (32, 64, 128, 256, 512, 1024) frames.\n");
    printf("  -b nB    nB (4096 -> 4T,\n");
    printf("               2048 -> 2T,\n");
    printf("               1024 ->  T,\n");
    printf("                512 ->  T/2,\n");
    printf("                256 ->  T/4,\n");
    printf("                128 ->  T/8,\n");
    printf("                 64 ->  T/16,\n");
    printf("                 32 ->  T/32,\n");
    printf("                 16 ->  T/64,\n");
    printf("                  8 ->  T/128,\n");
    printf("                  4 ->  T/256,\n");
    printf("                  2 ->  T/512,\n");
    printf("                  1 ->  T/1024).\n");
    printf("  -n       NB-IoT flag.\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.304 Section 7.1 Discontinuous Reception for paging
 */
int main(int argc, char *argv[])
{
    unsigned long long IMSI = 1010123456789LL;
    int UE_ID = 0;
    int T = 128;
    int bVal = 1024;
    int nFlag = 0;
    int nB;
    int N;
    int Ns;
    int i_s;
    int ch;
    int j;


    opterr = 0;
    while ((ch=getopt(argc, argv, "i:t:b:nh")) != -1)
    {
        switch ( ch )
        {
            case 'i':
                IMSI = imsiStrToInt( optarg );
                break;
            case 't':
                T = atoi( optarg );
                break;
            case 'b':
                bVal = atoi( optarg );
                break;
            case 'n':
                nFlag = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ( nFlag )
    {
        if ((T !=  128) &&
            (T !=  256) &&
            (T !=  512) &&
            (T != 1024))
        {
            printf("ERR: incorrect T %d\n\n", T);
            return -1;
        }
    }
    else
    {
        if ((T !=  32) &&
            (T !=  64) &&
            (T != 128) &&
            (T != 256))
        {
            printf("ERR: incorrect T %d\n\n", T);
            return -1;
        }
    }

    if ( nFlag )
    {
        if ((bVal !=    1) &&
            (bVal !=    2) &&
            (bVal !=    4) &&
            (bVal !=    8) &&
            (bVal !=   16) &&
            (bVal !=   32) &&
            (bVal !=   64) &&
            (bVal !=  128) &&
            (bVal !=  256) &&
            (bVal !=  512) &&
            (bVal != 1024) &&
            (bVal != 2048) &&
            (bVal != 4096))
        {
            printf("ERR: incorrect nB %d\n\n", bVal);
            return -1;
        }
    }
    else
    {
        if ((bVal !=   32) &&
            (bVal !=   64) &&
            (bVal !=  128) &&
            (bVal !=  256) &&
            (bVal !=  512) &&
            (bVal != 1024) &&
            (bVal != 2048) &&
            (bVal != 4096))
        {
            printf("ERR: incorrect nB %d\n\n", bVal);
            return -1;
        }
    }


    UE_ID = (( nFlag ) ? (IMSI & 0xFFF) : (IMSI & 0x3FF));
    nB = ((T * bVal) / 1024);
    N = ((T > nB) ? nB : T);
    if (0 == N)
    {
        printf("ERR: incorrect N %d\n\n", N);
        return -1;
    }
    Ns = (((nB / T) > 1) ? (nB / T) : 1);
    if ((Ns != 1) && (Ns != 2) && (Ns != 4))
    {
        printf("ERR: incorrect Ns %d\n\n", Ns);
        return -1;
    }
    i_s = (FLOOR(UE_ID, N) % Ns);

    printf("\n");
    printf("IMSI  = %015llu\n", IMSI);
    printf("UE_ID = %d\n", UE_ID);
    printf("T     = %d\n", T);
    for (j=13; j>0; j--)
    {
        if ((bVal >> (j-1)) == 0x1)
        {
            printf("nB    = %d (%s)\n", nB, _nBStr[13-j]);
            break;
        }
    }
    printf("N     = %d\n", N);
    printf("Ns    = %d\n", Ns);
    printf("i_s   = %d\n", i_s);
    printf("\n");

    show_PF_PO(
        UE_ID,
        T,
        N,
        Ns,
        i_s
    );

    return 0;
}

