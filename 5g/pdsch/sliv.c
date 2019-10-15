#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


void decodeSLIV(int SLIV, int *S, int *L)
{
    int S1, S2;
    int L1, L2;
    int SL1, SL2;
    int Q, R;

    Q = (SLIV / 14);
    R = (SLIV % 14);

    S1 = R;
    L1 = (Q + 1);
    S2 = (13 - R);
    L2 = (15 - Q);

    SL1 = (S1 + L1 - 1);
    SL2 = (S2 + L2 - 1);

    if ((SL1 < 14) && (SL2 >= 14))
    {
        (*S) = S1;
        (*L) = L1;
    }
    else if ((SL1 >= 14) && (SL2 < 14))
    {
        (*S) = S2;
        (*L) = L2;
    }
    else
    {
        (*S) = -1;
        (*L) = -1;
    }
}

int encodeSLIV(int S, int L)
{
    int SLIV;

    if ((L - 1) <= 7)
    {
        SLIV = ((14 * (L - 1)) + S);
    }
    else
    {
        SLIV = ((14 * (14 - L + 1)) + (14 - 1 - S));
    }

    return SLIV;
}

void Show_TypeA_SLIV(void)
{
    int S[4] = {0, 1, 2, 3};
    int L[12] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int SLIV;
    int i;
    int j;

    printf("PDSCH mapping type A:\n");
    printf(" S   L   SLIV\n");
    for (i=0; i<4; i++)
    {
        for (j=0; j<12; j++)
        {
            if (((S[i] + L[j]) >= 3) && ((S[i] + L[j]) <= 14))
            {
                SLIV = encodeSLIV(S[i], L[j]);
                printf("%2d  %2d   %3d\n", S[i], L[j], SLIV);
            }
        }
    }
    printf("\n");
}

void Show_TypeB_SLIV(void)
{
    int S[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int L[3] = {2, 4, 7};
    int SLIV;
    int i;
    int j;

    printf("PDSCH mapping type B:\n");
    printf(" S   L   SLIV\n");
    for (i=0; i<13; i++)
    {
        for (j=0; j<3; j++)
        {
            if (((S[i] + L[j]) >= 2) && ((S[i] + L[j]) <= 14))
            {
                SLIV = encodeSLIV(S[i], L[j]);
                printf("%2d  %2d   %3d\n", S[i], L[j], SLIV);
            }
        }
    }
    printf("\n");
}

/* 38.214 Section 5.1.2.1 Resource allocation in time domain */
int main(int argc, char *argv[])
{
    int SLIV;
    int S;
    int L;

    if ((3 == argc) && (0 == strcmp("-a", argv[1])))
    {
        if (0 == strcmp("A", argv[2]))
        {
            Show_TypeA_SLIV();
        }
        else if (0 == strcmp("B", argv[2]))
        {
            Show_TypeB_SLIV();
        }
        else
        {
            printf("ERR: wrong Type(%s)\n\n", argv[2]);
            return 0;
        }
    }
    else if ((3 == argc) && (0 == strcmp("-d", argv[1])))
    {
        SLIV = atoi( argv[2] );
        if ((SLIV < 0) || (SLIV > 127))
        {
            printf("ERR: wrong SLIV(%d)\n\n", SLIV);
            return 0;
        }

        decodeSLIV(SLIV, &S, &L);
        printf("\n");
        printf("S = %d\n", S);
        printf("L = %d\n", L);
        printf("\n");
    }
    else if ((4 == argc) && (0 == strcmp("-e", argv[1])))
    {
        S = atoi( argv[2] );
        if ((S < 0) || (S > 13))
        {
            printf("ERR: wrong S(%d)\n\n", S);
            return 0;
        }
        L = atoi( argv[3] );
        if ((L < 1) || (L > 14))
        {
            printf("ERR: wrong L(%d)\n\n", L);
            return 0;
        }

        if ((S + L) > 14)
        {
            printf("ERR: wrong S(%d), L(%d)\n\n", S, L);
            return 0;
        }

        SLIV = encodeSLIV(S, L);
        printf("\n");
        printf("SLIV = %d\n", SLIV);
        printf("\n");
    }
    else
    {
        printf("Usage: sliv -e S L\n");
        printf("  -e       Encode SLIV.\n");
        printf("   S       Symbol start (0 ~ 13).\n");
        printf("   L       Symbol length (1 ~ 14).\n");
        printf("\n");
        printf("Usage: sliv -d SLIV\n");
        printf("  -d       Decode SLIV.\n");
        printf("   SLIV    SLIV (0 ~ 127).\n");
        printf("\n");
        printf("Usage: sliv -a Type\n");
        printf("  -a       Show all of SLIVs.\n");
        printf("   Type    Type A or B.\n");
        printf("\n");
        return 0;
    }

    return 0;
}

