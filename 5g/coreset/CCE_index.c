#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


void help(void)
{
    printf("Usage: CCE_index [OPTION]...\n");
    printf("\n");
    printf("  -u              UE-specific search space.\n");
    printf("  -r n_RNTI       C-RNTI.\n");
    printf("  -p p            CORESET ID (0 ~ 11).\n");
    printf("  -n N_CCE_p      Number of CCEs in CORESET p (1 ~ 135).\n");
    printf("  -l L            Aggregation level (1, 2, 4, 8, 16).\n");
    printf("  -c n_CI         Carrier indicator.\n");
    printf("  -s n_u_s_f      Slot of search space (0 ~ 159).\n");
    printf("  -m M_L_s_n_CI   Number of PDCCH candidates (0 ~ 8).\n");
    printf("  -h              Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 10.1 UE procedure for determining physical downlink control channel assignment */
int main(int argc, char *argv[])
{
    int CSS = 1;
    int n_RNTI = 4096;
    int p = 0;
    int N_CCE_p = 16;
    int L = 4;
    int n_CI = 0;
    int n_u_s_f = 0;
    int M_L_s_n_CI = 2;

    int CCE;
    unsigned int Y_p[160];
    unsigned int Y_p_1;
    int A_p;
    int D;
    int M_L_s_max = 8;
    int m_s_n_CI;
    int ch;
    int i;
    int n;


    opterr = 0;
    while ((ch=getopt(argc, argv, "ur:p:n:l:c:s:m:h")) != -1)
    {
        switch ( ch )
        {
            case 'u':
                CSS = 0;
                break;
            case 'r':
                n_RNTI = atoi( optarg );
                if ((n_RNTI < 0) || (n_RNTI > 65535))
                {
                    printf("ERR: wrong C-RNTI %d\n\n", n_RNTI);
                    return -1;
                }
                break;
            case 'p':
                p = atoi( optarg );
                if ((p < 0) || (p > 11))
                {
                    printf("ERR: wrong CORESET ID %d\n\n", p);
                    return -1;
                }
                break;
            case 'n':
                N_CCE_p = atoi( optarg );
                if ((N_CCE_p < 1) || (N_CCE_p > 135))
                {
                    printf("ERR: wrong number of CCEs %d\n\n", N_CCE_p);
                    return -1;
                }
                break;
            case 'l':
                L = atoi( optarg );
                if ((L != 1) &&
                    (L != 2) &&
                    (L != 4) &&
                    (L != 8) &&
                    (L != 16))
                {
                    printf("ERR: wrong aggregation level %d\n\n", L);
                    return -1;
                }
                break;
            case 'c':
                n_CI = atoi( optarg );
                break;
            case 's':
                n_u_s_f = atoi( optarg );
                if ((n_u_s_f < 0) || (n_u_s_f > 159))
                {
                    printf("ERR: wrong search space slot %d\n\n", n_u_s_f);
                    return -1;
                }
                break;
            case 'm':
                M_L_s_n_CI = atoi( optarg );
                if (M_L_s_n_CI > M_L_s_max)
                {
                    printf("ERR: wrong number of PDCCH candidates %d\n\n", M_L_s_n_CI);
                    return -1;
                }
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if (N_CCE_p < L)
    {
        printf("ERR: N_CCE_p(%d) < L(%d)\n\n", N_CCE_p, L);
        return -1;
    }

    if ( CSS )
    {
        n_u_s_f = 0;
        Y_p[n_u_s_f] = 0;
        M_L_s_max = M_L_s_n_CI;  /* -> M_L_s_0 */
    }
    else
    {
        if (0 == (p % 3))
        {
            A_p = 39827;
        }
        else if (1 == (p % 3))
        {
            A_p = 39829;
        }
        else
        {
            A_p = 39839;
        }
        D = 65537;
        Y_p_1 = n_RNTI;
        for (n=0; n<n_u_s_f; n++)
        {
            Y_p[n] = ((A_p * Y_p_1) % D);
            Y_p_1 = Y_p[n];
        }
        Y_p[n_u_s_f] = ((A_p * Y_p_1) % D);
        M_L_s_max = M_L_s_n_CI;  /* -> maximum of M_L_s_n_CI */
    }

    printf("    n_RNTI = %d\n", n_RNTI);
    printf("         p = %3d (CORESET ID)\n", p);
    printf("   N_CCE_p = %3d (Number of CCEs)\n", N_CCE_p);
    printf("         L = %3d (Aggregation level)\n", L);
    printf("   n_u_s_f = %3d (Slot index in a frame)\n", n_u_s_f);
    printf("      n_CI = %3d (Carrier indicator)\n", n_CI);
    printf("M_L_s_n_CI = %3d (Number of PDCCH candidates)\n", M_L_s_n_CI);
    printf("\n");

    for (m_s_n_CI=0; m_s_n_CI<M_L_s_n_CI; m_s_n_CI++)
    {
        printf("PDCCH candidate[[1;31m%d[0m]\n", m_s_n_CI);
        printf(" + CCE[");
        for (i=0; i<L; i++)
        {
            CCE = ((L * ((Y_p[n_u_s_f] + DIV_FLOOR((m_s_n_CI * N_CCE_p), (L * M_L_s_max)) + n_CI) % DIV_FLOOR(N_CCE_p, L))) + i);
            printf("%d", CCE);
            if (i < (L - 1)) printf(", ");
            if (CCE >= N_CCE_p)
            {
                printf("\nERR: illegal CCE index %d\n\n", CCE);
                return -1;
            }
        }
        printf("]\n");
        printf("\n");
    }

    return 0;
}

