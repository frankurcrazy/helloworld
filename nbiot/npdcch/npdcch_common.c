
/*
* NPDCCH UE-specific search space:
*   Rmax = npdcch-NumRepetitions
*
* Type1-NPDCCH common search space:
*   Rmax = npdcch-NumRepetitionPaging
*
* Type2-NPDCCH common search space:
*   Rmax = npdcch-NumRepetitions-RA
*/
int _Rmax[12] = {
      1,   2,    4,    8,
     16,  32,   64,  128,
    256, 512, 1024, 2048
};

/*
* NPDCCH UE-specific search space:
*   G = npdcch-StartSF-USS
*
* Type2-NPDCCH common search space:
*   G = npdcch-StartSF-CSS-RA
*/
int _G[8] = {
/* 1.5  2   4   8  16  32  48   64 */
    3,  4,  8, 16, 32, 64, 96, 128
};
char *_GStr[] = {
    "1.5", "2", "4", "8", "16", "32", "48", "64"
};

/*
* NPDCCH UE-specific search space:
*   alpha = npdcch-Offset-USS
*
* Type2-NPDCCH common search space:
*   alpha = npdcch-Offset-RA
*/
int _alpha[4] = {
/*  0   1/8  1/4  3/8 */
    0,   1,   2,   3
};
char *_alphaStr[] = {
    "0", "1/8", "1/4", "3/8"
};


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


/*
* 36.213 Table 16.6-1, Table 16.6-2, Table 16.6-3
*/
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

/*
* 36.213 section 16.6
*/
int npdcch_k0(int Rmax, int G, int alpha, int *Tout, int *alphaTout)
{
    int k0 = -1;
    int T;
    int alphaT;

    T = ((Rmax * G) / 2);
    if ( Tout ) (*Tout) = T;

    if (T >= 4)
    {
        alphaT = ((alpha * T) / 8);
        if ( alphaTout ) (*alphaTout) = alphaT;

        if (alphaT < 10240)
        {
            k0 = alphaT;
        }
    }

    return k0;
}

