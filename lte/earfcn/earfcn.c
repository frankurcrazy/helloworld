#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define F_DL(f_dl_low, n_dl, n_offs_dl)  (f_dl_low + (0.1 * (n_dl - n_offs_dl)))
#define F_UL(f_ul_low, n_ul, n_offs_ul)  (f_ul_low + (0.1 * (n_ul - n_offs_ul)))


typedef enum
{
    INVALID_MODE = 0,
    FDD_MODE     = 1,
    TDD_MODE     = 2,
    NUM_OF_DUPLEX
} tDuplex;

typedef struct _tEarfcn
{
    int     duplex;
    int     band;
    double  f_dl_low;
    int     n_offs_dl;
    int     n_max_dl;
    double  f_ul_low;
    int     n_offs_ul;
    int     n_max_ul;
} tEarfcn;


static tEarfcn _earfcnArray[] = {
    /* duplex   band   f_dl_low   n_offs_dl   n_max_dl   f_ul_low   n_offs_ul  n_max_ul  */
    {       0,     0,         0,          0,         0,         0,          0,        0  },
    {       1,     1,      2110,          0,       599,      1920,      18000,    18599  },
    {       1,     2,      1930,        600,      1199,      1850,      18600,    19199  }, 
    {       1,     3,      1805,       1200,      1949,      1710,      19200,    19949  }, 
    {       1,     4,      2110,       1950,      2399,      1710,      19950,    20399  }, 
    {       1,     5,       869,       2400,      2649,       824,      20400,    20649  }, 
    {       1,     6,       875,       2650,      2749,       830,      20650,    20749  }, 
    {       1,     7,      2620,       2750,      3449,      2500,      20750,    21449  }, 
    {       1,     8,       925,       3450,      3799,       880,      21450,    21799  }, 
    {       1,     9,    1844.9,       3800,      4149,    1749.9,      21800,    22149  },
    {       1,    10,      2110,       4150,      4749,      1710,      22150,    22749  },
    {       1,    11,    1475.9,       4750,      4949,    1427.9,      22750,    22949  },
    {       1,    12,       729,       5010,      5179,       699,      23010,    23179  }, 
    {       1,    13,       746,       5180,      5279,       777,      23180,    23279  },
    {       1,    14,       758,       5280,      5379,       788,      23280,    23379  },
    {       0,    15,         0,          0,         0,         0,          0,        0  },
    {       0,    16,         0,          0,         0,         0,          0,        0  },
    {       1,    17,       734,       5730,      5849,       704,      23730,    23849  },
    {       1,    18,       860,       5850,      5999,       815,      23850,    23999  },
    {       1,    19,       875,       6000,      6149,       830,      24000,    24149  },
    {       1,    20,       791,       6150,      6449,       832,      24150,    24449  },
    {       1,    21,    1495.9,       6450,      6599,    1447.9,      24450,    24599  },
    {       1,    22,      3510,       6600,      7399,      3410,      24600,    25399  },
    {       1,    23,      2180,       7500,      7699,      2000,      25500,    25699  },
    {       1,    24,      1525,       7700,      8039,    1626.5,      25700,    26039  },
    {       1,    25,      1930,       8040,      8689,      1850,      26040,    26689  },
    {       0,    26,         0,          0,         0,         0,          0,        0  },
    {       0,    27,         0,          0,         0,         0,          0,        0  },
    {       0,    28,         0,          0,         0,         0,          0,        0  },
    {       0,    29,         0,          0,         0,         0,          0,        0  },
    {       0,    30,         0,          0,         0,         0,          0,        0  },
    {       0,    31,         0,          0,         0,         0,          0,        0  },
    {       0,    32,         0,          0,         0,         0,          0,        0  },
    {       2,    33,      1900,      36000,     36199,      1900,      36000,    36199  },
    {       2,    34,      2010,      36200,     36349,      2010,      36200,    36349  },
    {       2,    35,      1850,      36350,     36949,      1850,      36350,    36949  },
    {       2,    36,      1930,      36950,     37549,      1930,      36950,    37549  },
    {       2,    37,      1910,      37550,     37749,      1910,      37550,    37749  },
    {       2,    38,      2570,      37750,     38249,      2570,      37750,    38249  },
    {       2,    39,      1880,      38250,     38649,      1880,      38250,    38649  },
    {       2,    40,      2300,      38650,     39649,      2300,      38650,    39649  },
    {       2,    41,      2496,      39650,     41589,      2496,      39650,    41589  },
    {       2,    42,      3400,      41590,     43589,      3400,      41590,    43589  },
    {       2,    43,      3600,      43590,     45589,      3600,      43590,    45589  },
    {       0,    44,         0,          0,         0,         0,          0,        0  }
};

static int _earfcnArraySize = (sizeof( _earfcnArray ) / sizeof( tEarfcn ));


static void _showAllBand(void)
{
    int i;

    printf("Band   DL frequency(KHz)   N_DL   UL frequency(KHz)   N_UL\n");
    for (i=1; i<_earfcnArraySize; i++)
    {
        if ((i % 10) == 0)
        {
            printf("Band   DL frequency(KHz)   N_DL   UL frequency(KHz)   N_UL\n");
        }
        if (_earfcnArray[i].duplex > 0)
        {
            printf(
                " %2d        %8d       %5d       %8d       %5d\n",
                _earfcnArray[i].band,
                (int)(_earfcnArray[i].f_dl_low * 1000),
                _earfcnArray[i].n_offs_dl,
                (int)(_earfcnArray[i].f_ul_low * 1000),
                _earfcnArray[i].n_offs_ul
            );
        }
    }
    printf("\n");
}

static void _showBand(int band)
{
    tEarfcn *pEarfcn = &(_earfcnArray[ band ]);
    int f_high = 0;
    int f_low  = 0;
    int n_dl;
    int n_ul;

    if (FDD_MODE == pEarfcn->duplex)
    {
        f_high = F_DL(
                     pEarfcn->f_dl_low,
                     pEarfcn->n_max_dl,
                     pEarfcn->n_offs_dl
                 );
        f_low  = F_DL(
                     pEarfcn->f_dl_low,
                     pEarfcn->n_offs_dl,
                     pEarfcn->n_offs_dl
                 );

        //printf("%d -> %d\n", f_low, f_high);
        printf("[FDD] bandwidth %d MHz\n", (f_high - f_low + 1));
        printf("\n");
    }
    else if (TDD_MODE == pEarfcn->duplex)
    {
        f_high = F_DL(
                     pEarfcn->f_dl_low,
                     pEarfcn->n_max_dl,
                     pEarfcn->n_offs_dl
                 );
        f_low  = F_DL(
                     pEarfcn->f_dl_low,
                     pEarfcn->n_offs_dl,
                     pEarfcn->n_offs_dl
                 );

        //printf("%d -> %d\n", f_low, f_high);
        printf("[TDD] bandwidth %d MHz\n", (f_high - f_low + 1));
        printf("\n");
    }
    else
    {
        printf("invalid duplex mode %d\n", pEarfcn->duplex);
    }

    printf("[DL]\n");
    for (n_dl=pEarfcn->n_offs_dl; n_dl<pEarfcn->n_max_dl; n_dl+=100)
    {
        printf(
            "N_DL %5d: frequency %d MHz\n",
            n_dl,
            (int)F_DL(pEarfcn->f_dl_low, n_dl, pEarfcn->n_offs_dl)
        );
    }
    printf("\n");

    printf("[UL]\n");
    for (n_ul=pEarfcn->n_offs_ul; n_ul<pEarfcn->n_max_ul; n_ul+=100)
    {
        printf(
            "N_UL %5d: frequency %d MHz\n",
            n_ul,
            (int)F_UL(pEarfcn->f_ul_low, n_ul, pEarfcn->n_offs_ul)
        );
    }
    printf("\n");
}

/*
*  R10 36.104 Section 5.7.3 Carrier frequency and EARFCN
*/
int main(int argc, char *argv[])
{
    int band = 0;

    if (argc > 1)
    {
        if (0 == strcmp("-h", argv[1]))
        {
            printf("Usage: earfcn -h     Show this help.\n");
            printf("     : earfcn        Show all band info.\n");
            printf("     : earfcn BAND   Band number (0 ~ 44).\n");
            printf("\n");
            return 0;
        }

        band = atoi( argv[1] );
        if ((band < 1) ||
            ((band > 14) && (band < 17)) ||
            ((band > 25) && (band < 33)) ||
            (band > 43))
        {
            printf("ERR: invalid band %d\n", band);
            band = -1;
        }
    }

    if (band > 0)
    {
        _showBand( band );
    }
    else if (0 == band)
    {
        _showAllBand();
    }

    return 0;
}
