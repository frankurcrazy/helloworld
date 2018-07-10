
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

