#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


int Table_5_1_3_2_1[94] = {
    0,
   24,   32,   40,   48,   56,
   64,   72,   80,   88,   96,
  104,  112,  120,  128,  136,
  144,  152,  160,  168,  176,
  184,  192,  208,  224,  240,
  256,  272,  288,  304,  320,
  336,  352,  368,  384,  408,
  432,  456,  480,  504,  528,
  552,  576,  608,  640,  672,
  704,  736,  768,  808,  848,
  888,  928,  984, 1032, 1064,
 1128, 1160, 1192, 1224, 1256,
 1288, 1320, 1352, 1416, 1480,
 1544, 1608, 1672, 1736, 1800,
 1864, 1928, 2024, 2088, 2152,
 2216, 2280, 2408, 2472, 2536,
 2600, 2664, 2728, 2792, 2856,
 2976, 3104, 3240, 3368, 3496,
 3624, 3752, 3824
};


int max(int a, int b)
{
    return ((a > b) ? a : b);
}

void help(void)
{
    printf("Usage: ptrs [OPTION]...\n");
    printf("\n");
    printf("  -q Qm          Modulation order (2: QPSK, 4: 16QAM, 6: 64QAM, 8: 256QAM).\n");
    printf("  -r R           Target code rate x 1024.\n");
    printf("  -n n_PRB       PDSCH RB number (1 ~ 275).\n");
    printf("  -s N_sh_symb   PDSCH symbol number (2 ~ 14).\n");
    printf("  -d N_PRB_DMRS  PDSCH DM-RS RE number.\n");
    printf("  -o N_PRB_oh    xOverhead in PDSCH-ServingCellConfig (0, 6, 12, 18).\n");
    printf("  -v layer       Number of layer (1 ~ 8).\n");
    printf("  -h             Show the help message.\n");
    printf("\n");
}

/* 38.214 Section 5.1.3 Modulation order, target code rate, redundancy version
                        and transport block size determination */
int main(int argc, char *argv[])
{
    char modulation[8] = { '2', '5', '6', 'Q', 'A', 'M', 0, 0 };
    int Qm = 8;
    int R_p = 948;
    double R;
    int n_PRB = 273;
    int N_sh_symb = 12;
    int N_PRB_DMRS = 18;
    int N_PRB_oh = 0;
    int v = 2;

    int TBS = 0;
    int N_RB_sc = 12;
    int N_RE;
    int N_RE_p;
    int N_info;
    int N_info_p;
    int index;
    int C;
    int n;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "q:r:n:s:d:o:v:h")) != -1)
    {
        switch ( ch )
        {
            case 'q':
                Qm = atoi( optarg );
                if (1 == Qm)
                {
                    sprintf(modulation, "BPSK");
                }
                else if (2 == Qm)
                {
                    sprintf(modulation, "QPSK");
                }
                else if (4 == Qm)
                {
                    sprintf(modulation, "16QAM");
                }
                else if (6 == Qm)
                {
                    sprintf(modulation, "64QAM");
                }
                else if (8 == Qm)
                {
                    sprintf(modulation, "256QAM");
                }
                else
                {
                    printf("ERR: wrong value of Qm(%d)\n", Qm);
                    return 0;
                }
                break;
            case 'r':
                R_p = atoi( optarg );
                break;
            case 'n':
                n_PRB = atoi( optarg );
                if ((n_PRB < 1) || (n_PRB > 275))
                {
                    printf("ERR: wrong value of n_PRB(%d)\n", n_PRB);
                    return 0;
                }
                break;
            case 's':
                N_sh_symb = atoi( optarg );
                if ((N_sh_symb < 0) || (N_sh_symb > 14))
                {
                    printf("ERR: wrong value of N_sh_symb(%d)\n", N_sh_symb);
                    return 0;
                }
                break;
            case 'd':
                N_PRB_DMRS = atoi( optarg );
                break;
            case 'o':
                N_PRB_oh = atoi( optarg );
                if ((N_PRB_oh !=  0) &&
                    (N_PRB_oh !=  6) &&
                    (N_PRB_oh != 12) &&
                    (N_PRB_oh != 18))
                {
                    printf("ERR: wrong value of N_PRB_oh(%d)\n", N_PRB_oh);
                    return 0;
                }
                break;
            case 'v':
                v = atoi( optarg );
                if ((v < 0) || (v > 8))
                {
                    printf("ERR: wrong value of layers(%d)\n", v);
                    return 0;
                }
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    /* [1] For the PDSCH assigned by a PDCCH with DCI format 1_0 or format 1_1
     *     with CRC scrambled by C-RNTI, MCS-C-RNTI, TC-RNTI, CS-RNTI, or SI-RNTI,
     *     if Table 5.1.3.1-2 is used and, or a table other than Table 5.1.3.1-2 is used
     *     and, the UE shall, except if the transport block is disabled in DCI format 1_1,
     */

    N_RE_p = ((N_RB_sc * N_sh_symb) - N_PRB_DMRS - N_PRB_oh);
    if (156 < N_RE_p)
    {
        N_RE = (156 * n_PRB);
    }
    else
    {
        N_RE = (N_RE_p * n_PRB);
    }

    R = ((double)R_p / 1024);
    N_info = (int)(N_RE * R * Qm * v);
    if (N_info <= 3824)
    {
        n = max(3, (int)(floor( log2( N_info ) ) - 6));
        N_info_p = max(24, ((N_info >> n) << n));

        for (index=1; index<=93; index++)
        {
            if (Table_5_1_3_2_1[index] >= N_info_p)
            {
                TBS = Table_5_1_3_2_1[index];
                break;
            }
        }
    }
    else
    {
        n = ((int)floor( log2(N_info - 24) ) - 5);
        N_info_p = max(3840, ((int)round((double)(N_info - 24) / (0x1 << n)) << n));

        if (R <= 0.25)
        {
            C = (int)ceil((N_info_p + 24) / 3816);
            TBS = (8 * C * (int)ceil((N_info_p + 24) / (8 * C)));
        }
        else
        {
            if (N_info_p > 8424)
            {
                C = (int)ceil((N_info_p + 24) / 8424);
                TBS = (8 * C * (int)ceil((N_info_p + 24) / (8 * C)));
            }
            else
            {
                TBS = ((8 * (int)ceil((N_info_p + 24) / 8)) - 24);
            }
        }
    }

    /* [2] else if Table 5.1.3.1-2 is used and 28 <= I_MCS <= 31,
     */



    /* [3] else
     */



    printf("\n");
    printf("Qm = %d (%s)\n", Qm, modulation);
    printf("R = %d (code rate %.3f)\n", R_p, R);
    printf("n_PRB = %d (RBs)\n", n_PRB);
    printf("N_sh_symb = %d (symbols)\n", N_sh_symb);
    printf("N_PRB_DMRS = %d (REs)\n", N_PRB_DMRS);
    printf("N_PRB_oh = %d (xOverhead)\n", N_PRB_oh);
    printf("v = %d (layers)\n", v);
    printf("\n");

    printf("N_info = %d * %.3f * %d * %d = %d\n", N_RE, R, Qm, v, N_info);
    printf("TBS = [1;33m%d[0m bits (%d bytes)\n", TBS, (TBS >> 3));
    printf("\n");

    return 0;
}

