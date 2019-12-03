#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DEBUG_IMPORT (0)
#define DEBUG_DMRS   (0)


typedef enum _tPdschRs
{
    NONE  = 0,
    PDSCH = 1,
    DMRS  = 2,
    PTRS  = 3
} tPdschRs;


int RE[12][14] = {
/*    0,     1,     2,     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,    13  */
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, //11
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }, //10
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, // 9
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }, // 8
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, // 7
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }, // 6
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, // 5
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }, // 4
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, // 3
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }, // 2
{  NONE,  NONE, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH, PDSCH }, // 1
{  NONE,  NONE,  DMRS, PDSCH, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH, PDSCH,  DMRS, PDSCH, PDSCH }  // 0
};
int S = 2;
int L = 12;


void importPDSCH(int RE[12][14])
{
    FILE *pFile;
    char  string[32];
    int   l;
    int   j = 0;

    pFile = fopen("PDSCH_RS.txt", "r");
    if (NULL == pFile)
    {
        printf("WRN: cannot open file \"PDSCH_RS.txt\"\n");
        return;
    }

    memset(RE, 0, (12 * 14 * sizeof(int)));

    #if (DEBUG_IMPORT)
    printf("PDSCH_RS.txt:\n");
    #endif
    while ( !feof( pFile ) )
    {
        fgets(string, 32, pFile);
        if ( feof(pFile) )
        {
            break;
        }

        #if (DEBUG_IMPORT)
        printf("%s", string);
        #endif

        for (l=0; l<14; l++)
        {
            RE[j][l] = (string[l] - '0');
        }
        j++;
    }
    #if (DEBUG_IMPORT)
    printf("\n");
    #endif

    fclose( pFile );
}

void showResourceElement(int RE[12][14])
{
    int k;
    int l;
    int j = 0;

    for (k=11; k>=0; k--)
    {
        printf("%2d", k);
        for (l=0; l<14; l++)
        {
            if (DMRS == RE[j][l])
            {
                printf(" [1;33mDM[0m");
            }
            else if (PTRS == RE[j][l])
            {
                printf(" [1;31mPT[0m");
            }
            else if (PDSCH == RE[j][l])
            {
                printf("  *");
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

int overlap_dmrs(
    int  RE[12][14],
    int  S,
    int  L,
    int  begin,
    int  end,
    int *i,
    int *l_ref
)
{
    int inside_pdsch = 0;
    int dmrs_found;
    int k;
    int l;

    for (l=begin; l<=end; l++)
    {
        if ((l >= S) && (l <= (S + L - 1)))
        {
            dmrs_found = 0;
            for (k=0; k<12; k++)
            {
                if (DMRS == RE[k][l])
                {
                    dmrs_found = 1;
                    break;
                }
            }

            if ( dmrs_found )
            {
                (*i) = 1;
                if (DMRS == RE[k][l + 1])
                {
                    (*l_ref) = (l + 1);
                }
                else
                {
                    (*l_ref) = l;
                }

                #if (DEBUG_DMRS)
                printf("DM-RS at %d\n", (*l_ref));
                #endif
                return 1;
            }

            inside_pdsch = 1;
        }
    }

    return (( inside_pdsch ) ? 0 : 2);
}

void subcarrier_ptrs(int RE[12][14], int l, int K_PT_RS, int k_RE_ref, int k_RB_ref)
{
    int mapping[12] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int k;
    int i;

    for (i=0; i<12; i++)
    {
        k = (k_RE_ref + (((i * K_PT_RS) + k_RB_ref) * 12));

        if ((k >= 0) && (k <= 11))
        {
            RE[mapping[k]][l] = PTRS;
        }
    }
}

int max(int a, int b)
{
    return ((a > b) ? a : b);
}

void help(void)
{
    printf("Usage: ptrs [OPTION]...\n");
    printf("\n");
    printf("  -l L_PT_RS   Time density (1, 2, 4).\n");
    printf("  -k K_PT_RS   Frequency density (2, 4).\n");
    printf("  -o k_RE_ref  Resource element offset (0 ~ 11).\n");
    printf("  -r n_RNTI    PDSCH RNTI (1 ~ 65535).\n");
    printf("  -n N_RB      PDSCH RB number (1 ~ 275).\n");
    printf("  -i flag      Import file: PDSCH_RS.txt.\n");
    printf("  -h           Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 7.4.1.2 Phase-tracking reference signals for PDSCH */
int main(int argc, char *argv[])
{
    int L_PT_RS = 2;
    int K_PT_RS = 2;
    int k_RE_ref = 0;
    int k_RB_ref;
    int n_RNTI = 4096;
    int N_RB = 50;
    int iFlag = 1;
    int dmrs_symbol;
    int begin;
    int end;
    int l_ref;
    int l;
    int i;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "l:k:o:r:n:i:h")) != -1)
    {
        switch ( ch )
        {
            case 'l':
                L_PT_RS = atoi( optarg );
                if ((L_PT_RS != 1) && (L_PT_RS != 2) && (L_PT_RS != 4))
                {
                    printf("ERR: wrong value of L_PT_RS(%d)\n", L_PT_RS);
                    return 0;
                }
                break;
            case 'k':
                K_PT_RS = atoi( optarg );
                if ((K_PT_RS != 2) && (K_PT_RS != 4))
                {
                    printf("ERR: wrong value of K_PT_RS(%d)\n", K_PT_RS);
                    return 0;
                }
                break;
            case 'o':
                k_RE_ref = atoi( optarg );
                if ((k_RE_ref < 0) || (k_RE_ref > 11))
                {
                    printf("ERR: wrong value of k_RE_ref(%d)\n", k_RE_ref);
                    return 0;
                }
                break;
            case 'r':
                n_RNTI = atoi( optarg );
                if ((n_RNTI < 1) || (n_RNTI > 65535))
                {
                    printf("ERR: wrong value of n_RNTI(%d)\n", n_RNTI);
                    return 0;
                }
                break;
            case 'n':
                N_RB = atoi( optarg );
                if ((N_RB < 1) || (N_RB > 275))
                {
                    printf("ERR: wrong value of N_RB(%d)\n", N_RB);
                    return 0;
                }
                break;
            case 'i':
                iFlag = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ( iFlag )
    {
        importPDSCH( RE );
    }

    i = 0;
    l_ref = 0;
    k_RB_ref = ((0 == (N_RB % K_PT_RS)) ? (n_RNTI % K_PT_RS) : (n_RNTI % (N_RB % K_PT_RS)));

    do
    {
        begin = max((l_ref + ((i - 1) * L_PT_RS) + 1), l_ref);
        end = (l_ref + (i * L_PT_RS));

        #if (DEBUG_DMRS)
        printf("check interval %d - %d\n", begin, end);
        #endif

        dmrs_symbol = overlap_dmrs(
                          RE,
                          S,
                          L,
                          begin,
                          end,
                          &i,
                          &l_ref
                      );
        l = (l_ref + (i * L_PT_RS));

        if (dmrs_symbol != 1)
        {
            if (dmrs_symbol == 0)
            {
                subcarrier_ptrs(RE, l, K_PT_RS, k_RE_ref, k_RB_ref);
            }
            i++;
            l = (l_ref + (i * L_PT_RS));
        }

        if (l > (S + L - 1))
        {
            break;
        }
    } while (1);

    printf("\n");
    printf("L_PT_RS = %d\n", L_PT_RS);
    printf("K_PT_RS = %d\n", K_PT_RS);
    printf("k_RE_ref = %d\n", k_RE_ref);
    printf("k_RB_ref = %d\n", k_RB_ref);
    printf("n_RNTI = %d\n", n_RNTI);
    printf("N_RB = %d\n", N_RB);
    printf("S = %d\n", S);
    printf("L = %d\n", L);
    printf("\n");


    showResourceElement( RE );

    printf("    0  1  2  3  4  5  6  7  8  9 10 11 12 13\n");
    printf("\n");

    return 0;
}

