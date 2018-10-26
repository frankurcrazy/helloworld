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

char *_nBStr[8] = {
    "4T", "2T", "T", "T/2", "T/4", "T/8", "T/16", "T/32"
};


unsigned long long imsiStrToInt(char *pStr)
{
    unsigned long long IMSI = 0;
    unsigned long long table[] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000,
        1000000000,
        10000000000,
        100000000000,
        1000000000000,
        10000000000000,
        100000000000000
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


    printf("------+--------------\n");
    printf("Frame | Subframe(s)\n");
    printf("------+--------------\n");
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
    printf("------+--------------\n");
    printf("\n");
}

void help(void)
{
    printf("Usage: paging [OPTION]...\n");
    printf("\n");
    printf("  -i IMSI  IMSI string (15 characters).\n");
    printf("  -t T     DRX cycle (32, 64, 128, 256) frames.\n");
    printf("  -b nB    nB ( 128 -> 4T,\n");
    printf("                 64 -> 2T,\n");
    printf("                 32 ->  T,\n");
    printf("                 16 ->  T/2,\n");
    printf("                  8 ->  T/4,\n");
    printf("                  4 ->  T/8,\n");
    printf("                  2 ->  T/16,\n");
    printf("                  1 ->  T/32).\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.304 Section 7.1 Discontinuous Reception for paging
 */
int main(int argc, char *argv[])
{
    unsigned long long IMSI = 1010123456789ll;
    int UE_ID = 0;
    int T = 128;
    int bVal = 32;
    int nB;
    int N;
    int Ns;
    int i_s;
    int ch;
    int j;


    opterr = 0;
    while ((ch=getopt(argc, argv, "i:t:b:h")) != -1)
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
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((T != 32) && (T != 64) && (T != 128) && (T != 256))
    {
        printf("ERR: incorrect T %d\n\n", T);
        return -1;
    }

    if ((bVal !=   1) &&
        (bVal !=   2) &&
        (bVal !=   4) &&
        (bVal !=   8) &&
        (bVal !=  16) &&
        (bVal !=  32) &&
        (bVal !=  64) &&
        (bVal != 128))
    {
        printf("ERR: incorrect nB %d\n\n", bVal);
        return -1;
    }


    UE_ID = (IMSI & 0x3FF);
    nB = ((T * bVal) / 32);
    N = ((T > nB) ? nB : T);
    Ns = (((nB / T) > 1) ? (nB / T) : 1);
    i_s = (FLOOR(UE_ID, N) % Ns);

    printf("\n");
    printf("IMSI  = %015llu\n", IMSI);
    printf("UE_ID = %d\n", UE_ID);
    printf("T     = %d\n", T);
    for (j=8; j>0; j--)
    {
        if ((bVal >> (j-1)) == 0x1)
        {
            printf("nB    = %d (%s)\n", nB, _nBStr[8-j]);
            break;
        }
    }
    printf("N     = %d\n", N);
    printf("Ns    = %d\n", Ns);
    printf("i_s   = %d\n", i_s);
    printf("\n");

    if ((Ns != 1) && (Ns != 2) && (Ns != 4))
    {
        printf("ERR: incorrect Ns %d\n\n", Ns);
        return -1;
    }

    show_PF_PO(
        UE_ID,
        T,
        N,
        Ns,
        i_s
    );

    return 0;
}

