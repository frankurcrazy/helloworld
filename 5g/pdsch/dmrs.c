#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DEBUG_lb  (0)
#define DEBUG_k   (0)
#define DEBUG_RE  (0)


typedef enum _tPdschRs
{
    NONE  = 0,
    PDSCH = 1,
    DMRS  = 2,
    PTRS  = 3
} tPdschRs;


typedef struct _tDmrsPos
{
    int lb[4];
    int lbNum;
    int symbol[14];
} tDmrsPos;


int Table_7_4_1_1_2_1(int p)
{
    int delta[8] = {0, 0, 1, 1, 0, 0, 1, 1};
    int i;

    i = (p - 1000);
    if ((i < 0) || (i > 7))
    {
        printf("[1;31mERR:[0m wrong antenna port %d\n", p);
        return 0;
    }

    return delta[i];
}

int Table_7_4_1_1_2_2(int p)
{
    int delta[12] = {0, 0, 2, 2, 4, 4, 0, 0, 2, 2, 4, 4};
    int i;

    i = (p - 1000);
    if ((i < 0) || (i > 11))
    {
        printf("[1;31mERR:[0m wrong antenna port %d\n", p);
        return 0;
    }

    return delta[i];
}

void Table_7_4_1_1_2_3(
    int type,
    int S,
    int l0,
    int ld,
    int apos,
    tDmrsPos *pPos
)
{
    int lp = 0;
    int l;
    int i;

    memset(pPos, 0, sizeof( tDmrsPos ));

    if (0 == type)
    {
        switch ( ld )
        {
            case 3:
                if (2 == l0)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                pPos->lb[0] = l0;
                pPos->lbNum = 1;
                break;
            case 8:
            case 9:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 7;
                    pPos->lbNum = 2;
                }
                break;
            case 10:
            case 11:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 9;
                    pPos->lbNum = 2;
                }
                else
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 6;
                    pPos->lb[2] = 9;
                    pPos->lbNum = 3;
                }
                break;
            case 12:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 9;
                    pPos->lbNum = 2;
                }
                else if (2 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 6;
                    pPos->lb[2] = 9;
                    pPos->lbNum = 3;
                }
                else
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 5;
                    pPos->lb[2] = 8;
                    pPos->lb[3] = 11;
                    pPos->lbNum = 4;
                }
                break;
            case 13:
            case 14:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 11;
                    pPos->lbNum = 2;
                }
                else if (2 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 7;
                    pPos->lb[2] = 11;
                    pPos->lbNum = 3;
                }
                else
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 5;
                    pPos->lb[2] = 8;
                    pPos->lb[3] = 11;
                    pPos->lbNum = 4;
                }
                break;
            case 2:
            default:
                ;
        }
    }
    else
    {
        switch ( ld )
        {
            case 2:
            case 4:
                if ((0 == apos) || (1 == apos))
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                break;
            case 6:
            case 7:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 4;
                    pPos->lbNum = 2;
                }
                break;
            case 3:
            case 5:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            default:
                ;
        }
    }

    #if (DEBUG_lb)
    printf("lb:");
    #endif
    for (i=0; i<pPos->lbNum; i++)
    {
        if (0 == type)
        {
            l = (pPos->lb[i] + lp);
        }
        else
        {
            l = (S + (pPos->lb[i] + lp));
        }
        if ((l < 0) || (l > 13))
        {
            printf("[1;31mERR:[0m wrong value of l(%d)\n", l);
            continue;
        }
        pPos->symbol[l] = 1;
        #if (DEBUG_lb)
        printf(" %d", pPos->lb[i]);
        #endif
    }
    #if (DEBUG_lb)
    printf("\n");
    #endif
}

void Table_7_4_1_1_2_4(
    int type,
    int S,
    int l0,
    int ld,
    int apos,
    tDmrsPos *pPos
)
{
    int lp[2] = {0, 1};
    int l;
    int i;
    int j;

    memset(pPos, 0, sizeof( tDmrsPos ));

    if (0 == type)
    {
        switch ( ld )
        {
            case 4:
                if (2 == l0)
                {
                    if ((0 == apos) || (1 == apos))
                    {
                        pPos->lb[0] = l0;
                        pPos->lbNum = 1;
                    }
                }
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                if ((0 == apos) || (1 == apos))
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                break;
            case 10:
            case 11:
            case 12:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 8;
                    pPos->lbNum = 2;
                }
                break;
            case 13:
            case 14:
                if (0 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                else if (1 == apos)
                {
                    pPos->lb[0] = l0;
                    pPos->lb[1] = 10;
                    pPos->lbNum = 2;
                }
                break;
            default:
                ;
        }
    }
    else
    {
        switch ( ld )
        {
            case 6:
            case 7:
                if ((0 == apos) || (1 == apos))
                {
                    pPos->lb[0] = l0;
                    pPos->lbNum = 1;
                }
                break;
            default:
                ;
        }
    }

    #if (DEBUG_lb)
    printf("lb:");
    #endif
    for (i=0; i<pPos->lbNum; i++)
    {
        for (j=0; j<((0 == i) ? 2 : 1); j++)
        {
            if (0 == type)
            {
                l = (pPos->lb[i] + lp[j]);
            }
            else
            {
                l = (S + (pPos->lb[i] + lp[j]));
            }
            if ((l < 0) || (l > 13))
            {
                printf("[1;31mERR:[0m wrong value of l(%d)\n", l);
                continue;
            }
            pPos->symbol[l] = 1;
        }
        #if (DEBUG_lb)
        printf(" %d", pPos->lb[i]);
        #endif
    }
    #if (DEBUG_lb)
    printf("\n");
    #endif
}

void showPDSCH(int RE[12][14], int S, int L)
{
    int k;
    int l;
    int j = 0;

    for (k=11; k>=0; k--)
    {
        printf("%2d", k);
        for (l=0; l<14; l++)
        {
            if ((l >= S) && (l <= (S + L - 1)))
            {
                if (RE[j][l] >= 0)
                {
                    #if 1
                    printf(" [1;33mDM[0m");
                    #else
                    printf(" [1;33m%2d[0m", l);
                    #endif
                }
                else
                {
                    printf("  *");
                }
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
        j++;
    }
}

void exportPDSCH(int RE[12][14], int S, int L)
{
    FILE *pFile;
    int l;
    int j;

    pFile = fopen("PDSCH_RS.txt", "w");
    if ( pFile )
    {
        for (j=0; j<12; j++)
        {
            for (l=0; l<14; l++)
            {
                if ((l >= S) && (l <= (S + L - 1)))
                {
                    if (RE[j][l] >= 0)
                    {
                        fprintf(pFile, "%d", DMRS);
                    }
                    else
                    {
                        fprintf(pFile, "%d", PDSCH);
                    }
                }
                else
                {
                    fprintf(pFile, "%d", NONE);
                }
            }
            fprintf(pFile, "\n");
        }
        fclose( pFile );
    }
}

void help(void)
{
    printf("Usage: dmrs [OPTION]...\n");
    printf("\n");
    printf("  -s S        PDSCH start symbol (0 ~ 13).\n");
    printf("  -l L        PDSCH symbol number (1 ~ 14).\n");
    printf("  -t type     PDSCH mapping type (0: A, 1: B).\n");
    printf("  -d type     DM-RS type (1, 2).\n");
    printf("  -m symbol   DM-RS max. length (1, 2 symbol).\n");
    printf("  -p pos      DM-RS type A position (2, 3).\n");
    printf("  -q pos      DM-RS additional position (0, 1, 2, 3).\n");
    printf("  -a port     Antenna port (1000 ~ 1008).\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 7.4.1.1 Demodulation reference signals for PDSCH */
int main(int argc, char *argv[])
{
    int S = 0;
    int L = 14;
    int mappingType = 0;
    int dmrsType = 1;
    int maxLen = 1;
    int l0;
    int ld;
    int dmrs_TypeA_poisition = 2;
    int dmrs_additional_position = 2;
    int antenna = 1000;
    int delta;

    tDmrsPos  dmrsPos;
    int RE[12][14];
    int k[6];
    int l;
    int i;
    int j;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "s:l:t:d:m:p:q:a:h")) != -1)
    {
        switch ( ch )
        {
            case 's':
                S = atoi( optarg );
                break;
            case 'l':
                L = atoi( optarg );
                break;
            case 't':
                mappingType = atoi( optarg );
                break;
            case 'd':
                dmrsType = atoi( optarg );
                break;
            case 'm':
                maxLen = atoi( optarg );
                break;
            case 'p':
                dmrs_TypeA_poisition = atoi( optarg );
                if ((dmrs_TypeA_poisition != 2) && (dmrs_TypeA_poisition != 3))
                {
                    printf("ERR: wrong DM-RS typeA position %d\n", dmrs_TypeA_poisition);
                    return 0;
                }
                break;
            case 'q':
                dmrs_additional_position = atoi( optarg );
                if ((dmrs_additional_position < 0) || (dmrs_additional_position > 3))
                {
                    printf("ERR: wrong DM-RS additional position %d\n", dmrs_additional_position);
                    return 0;
                }
                if ((dmrs_additional_position == 3) && (dmrs_TypeA_poisition != 2))
                {
                    printf("ERR: wrong DM-RS additional position %d\n", dmrs_additional_position);
                    return 0;
                }
                break;
            case 'a':
                antenna = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if (0 == mappingType)
    {
        /* PDSCH mapping type A */
        if ((S < 0) || (S > 3))
        {
            printf("ERR: wrong value of S(%d)\n\n", S);
            return 0;
        }
        if ((L < 3) || (L > 14))
        {
            printf("ERR: wrong value of L(%d) for mapping typeA\n\n", L);
            return 0;
        }
        l0 = dmrs_TypeA_poisition;
        ld = (S + L);
    }
    else if (1 == mappingType)
    {
        /* PDSCH mapping type B */
        if ((S < 0) || (S > 12))
        {
            printf("ERR: wrong value of S(%d)\n\n", S);
            return 0;
        }
        if ((L != 2) && (L != 4) && (L != 7))
        {
            printf("ERR: wrong value of L(%d) for mapping typeB\n\n", L);
            return 0;
        }
        l0 = 0;
        ld = L;
    }
    else
    {
        printf("ERR: wrong PDSCH mapping type %d\n\n", mappingType);
        return 0;
    }

    if (1 == maxLen)
    {
        Table_7_4_1_1_2_3(
            mappingType,
            S,
            l0,
            ld,
            dmrs_additional_position,
            &dmrsPos
        );
    }
    else
    {
        Table_7_4_1_1_2_4(
            mappingType,
            S,
            l0,
            ld,
            dmrs_additional_position,
            &dmrsPos
        );
    }

    if (1 == dmrsType)
    {
        delta = Table_7_4_1_1_2_1( antenna );
        k[0] = (delta);
        k[1] = (2 + delta);
        k[2] = (4 + delta);
        k[3] = (4 + 2 + delta);
        k[4] = (8 + delta);
        k[5] = (8 + 2 + delta);
        #if (DEBUG_k)
        printf("k: %d %d %d %d %d %d\n", k[0], k[1], k[2], k[3], k[4], k[5]);
        #endif
    }
    else
    {
        delta = Table_7_4_1_1_2_2( antenna );
        k[0] = (delta);
        k[1] = (1 + delta);
        k[2] = (6 + delta);
        k[3] = (6 + 1 + delta);
        k[4] = -1;
        k[5] = -1;
        #if (DEBUG_k)
        printf("k: %d %d %d %d\n", k[0], k[1], k[2], k[3]);
        #endif
    }

    for (i=0; i<14; i++)
    {
        l = i;
        if ((l >= S) && (l <= (S + L - 1)))
        {
            for (j=0; j<12; j++)
            {
                if ((j == k[0]) ||
                    (j == k[1]) ||
                    (j == k[2]) ||
                    (j == k[3]) ||
                    (j == k[4]) ||
                    (j == k[5]))
                {
                    RE[11-j][i] = (( dmrsPos.symbol[l] ) ? l : -1);
                }
                else
                {
                    RE[11-j][i] = -1;
                }
            }
        }
        else
        {
            for (j=0; j<12; j++)
            {
                RE[11-j][i] = -1;
            }
        }
    }

    printf("\n");
    printf("PDSCH mapping type = %c\n", (mappingType == 0) ? 'A' : 'B');
    printf("PDSCH S = %d\n", S);
    printf("PDSCH L = %d\n", L);
    printf("DM-RS type = %d\n", dmrsType);
    printf("DM-RS max. length = %d\n", maxLen);
    if (0 == mappingType)
    {
    printf("DM-RS typeA position = %d\n", dmrs_TypeA_poisition);
    }
    printf("DM-RS additional position = %d\n", dmrs_additional_position);
    printf("Antenna port = %d\n", antenna);
    printf("\n");

    #if (DEBUG_RE)
    for (j=0; j<12; j++)
    {
        for (i=0; i<14; i++)
        {
            printf(" %2d", RE[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
    #endif

    showPDSCH(RE, S, L);

    printf("    0  1  2  3  4  5  6  7  8  9 10 11 12 13\n");
    printf("\n");


    exportPDSCH(RE, S, L);

    return 0;
}

