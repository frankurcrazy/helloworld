#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


int Table_6_3_3_2_1(int L_RA, double f_PRACH, int f_PUSCH, int *kb)
{
    int N_RA_RB = 0;
    int k = 0;

    if (839 == L_RA)
    {
        if (1.25 == f_PRACH)
        {
            if (15 == f_PUSCH)
            {
                N_RA_RB = 6;
                k = 7;
            }
            else if (30 == f_PUSCH)
            {
                N_RA_RB = 3;
                k = 1;
            }
            else if (60 == f_PUSCH)
            {
                N_RA_RB = 2;
                k = 133;
            }
        }
        else if (5 == f_PRACH)
        {
            if (15 == f_PUSCH)
            {
                N_RA_RB = 24;
                k = 12;
            }
            else if (30 == f_PUSCH)
            {
                N_RA_RB = 12;
                k = 10;
            }
            else if (60 == f_PUSCH)
            {
                N_RA_RB = 6;
                k = 7;
            }
        }
    }
    else if (139 == L_RA)
    {
        if (15 == f_PRACH)
        {
            if (15 == f_PUSCH)
            {
                N_RA_RB = 12;
                k = 2;
            }
            else if (30 == f_PUSCH)
            {
                N_RA_RB = 6;
                k = 2;
            }
            else if (60 == f_PUSCH)
            {
                N_RA_RB = 3;
                k = 2;
            }
        }
        else if (30 == f_PRACH)
        {
            if (15 == f_PUSCH)
            {
                N_RA_RB = 24;
                k = 2;
            }
            else if (30 == f_PUSCH)
            {
                N_RA_RB = 12;
                k = 2;
            }
            else if (60 == f_PUSCH)
            {
                N_RA_RB = 6;
                k = 2;
            }
        }
        else if (60 == f_PRACH)
        {
            if (60 == f_PUSCH)
            {
                N_RA_RB = 12;
                k = 2;
            }
            else if (120 == f_PUSCH)
            {
                N_RA_RB = 6;
                k = 2;
            }
        }
        else if (120 == f_PRACH)
        {
            if (60 == f_PUSCH)
            {
                N_RA_RB = 24;
                k = 2;
            }
            else if (120 == f_PUSCH)
            {
                N_RA_RB = 12;
                k = 2;
            }
        }
    }

    if ( kb ) (*kb) = k;

    return N_RA_RB;
}

void help(void)
{
    printf("Usage: preamble_format [OPTION]...\n");
    printf("\n");
    printf("  -f value   Preamble format (0, 1, 2, 3 for L839).\n");
    printf("                             (A1, A2, A3, B1, B2, B3, B4, C0, C2 for L139).\n");
    printf("  -a value   PRACH subcarrier spacing (15, 30, 60, 120).\n");
    printf("  -s value   PUSCH subcarrier spacing (15, 30, 60, 120).\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 6.3.3 Physical random-access channel */
int main(int argc, char *argv[])
{
    char *format = "A2";
    double f_PRACH = 15;
    int f_PUSCH = 15;
    int u = 0;
    int k = 64;

    int samples = 0;
    int symbols[2] = {0};
    double subcarriers[2] = {0};
    double length = 0;
    int L_RA;
    int N_RA_CP;
    int N_u;
    int N_gap;
    int N_RA_RB;
    int kb;
    int ch;
    int i;


    opterr = 0;
    while ((ch=getopt(argc, argv, "f:a:s:h")) != -1)
    {
        switch ( ch )
        {
            case 'f':
                format = optarg;
                break;
            case 'a':
                f_PRACH = atoi( optarg );
                if (f_PRACH == 15)
                {
                    u = 0;
                }
                else if (f_PRACH == 30)
                {
                    u = 1;
                }
                else if (f_PRACH == 60)
                {
                    u = 2;
                }
                else if (f_PRACH == 120)
                {
                    u = 3;
                }
                else
                {
                    printf("ERR: wrong PRACH subcarrier spacing %s\n\n", optarg);
                    return 0;
                }
                break;
            case 's':
                f_PUSCH = atoi( optarg );
                if ((f_PUSCH != 15) &&
                    (f_PUSCH != 30) &&
                    (f_PUSCH != 60) &&
                    (f_PUSCH != 120))
                {
                    printf("ERR: wrong PUSCH subcarrier spacing %s\n\n", optarg);
                    return 0;
                }
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if (('0' == format[0]) ||
        ('1' == format[0]) ||
        ('2' == format[0]) ||
        ('3' == format[0])) 
    {
        if (0 == strcmp("0", format))
        {
            L_RA = 839;
            f_PRACH = 1.25;
            N_RA_CP = 3168;
            N_u = (24576);
            N_gap = 2976;
        }
        else if (0 == strcmp("1", format))
        {
            L_RA = 839;
            f_PRACH = 1.25;
            N_RA_CP = 21024;
            N_u = (2 * 24576);
            N_gap = 21984;
        }
        else if (0 == strcmp("2", format))
        {
            L_RA = 839;
            f_PRACH = 1.25;
            N_RA_CP = 4688;
            N_u = (4 * 24576);
            N_gap = 29264;
        }
        else if (0 == strcmp("3", format))
        {
            L_RA = 839;
            f_PRACH = 5;
            N_RA_CP = 3168;
            N_u = (4 * 6144);
            N_gap = 2976;
        }

        N_RA_RB = Table_6_3_3_2_1(L_RA, f_PRACH, f_PUSCH, &kb);
        if (0 == N_RA_RB)
        {
            printf("ERR: wrong N_RA_RB %d\n\n", N_RA_RB);
            return 0;
        }

        samples = ((N_RA_CP * k) + (N_u * k));
        length  = (((double)samples * 0.509) / 1000);
    }
    else if (('A' == format[0]) ||
             ('B' == format[0]) ||
             ('C' == format[0]))
    {
        if (0 == strcmp("A1", format))
        {
            L_RA = 139;
            N_RA_CP = 288;
            N_u = (2 * 2048);
            N_gap = 0;
        }
        else if (0 == strcmp("A2", format))
        {
            L_RA = 139;
            N_RA_CP = 576;
            N_u = (4 * 2048);
            N_gap = 0;
        }
        else if (0 == strcmp("A3", format))
        {
            L_RA = 139;
            N_RA_CP = 864;
            N_u = (6 * 2048);
            N_gap = 0;
        }
        else if (0 == strcmp("B1", format))
        {
            L_RA = 139;
            N_RA_CP = 216;
            N_u = (2 * 2048);
            N_gap = 72;
        }
        else if (0 == strcmp("B2", format))
        {
            L_RA = 139;
            N_RA_CP = 360;
            N_u = (4 * 2048);
            N_gap = 216;
        }
        else if (0 == strcmp("B3", format))
        {
            L_RA = 139;
            N_RA_CP = 504;
            N_u = (6 * 2048);
            N_gap = 360;
        }
        else if (0 == strcmp("B4", format))
        {
            L_RA = 139;
            N_RA_CP = 936;
            N_u = (12 * 2048);
            N_gap = 792;
        }
        else if (0 == strcmp("C0", format))
        {
            L_RA = 139;
            N_RA_CP = 1240;
            N_u = (2048);
            N_gap = 1096;
        }
        else if (0 == strcmp("C2", format))
        {
            L_RA = 139;
            N_RA_CP = 2048;
            N_u = (4 * 2048);
            N_gap = 2912;
        }

        N_RA_RB = Table_6_3_3_2_1(L_RA, f_PRACH, f_PUSCH, &kb);
        if (0 == N_RA_RB)
        {
            printf("ERR: wrong N_RA_RB %d\n\n", N_RA_RB);
            return 0;
        }

        samples = (((N_RA_CP * k) >> u) + ((N_u * k) >> u));
        length  = (((double)samples * 0.509) / 1000);
    }
    else
    {
        printf("ERR: wrong preamble format %s\n\n", format);
        return 0;
    }

    symbols[0] = DIV_CEIL(N_u, 2048);
    symbols[1] = DIV_CEIL((N_RA_CP + N_u + N_gap), (144 + 2048));
    subcarriers[0] = (L_RA * f_PRACH);
    subcarriers[1] = (N_RA_RB * 12 * f_PUSCH);

    printf("format  = %s\n", format);
    printf("length  = %f (us)\n", length);
    printf("samples = %d\n", samples);
    printf("symbols = %d\n", symbols[0]);
    printf("N_RA_RB = %d\n", N_RA_RB);
    printf("df_RA   = %.2f (KHz)\n", f_PRACH);
    printf("df      = %d (KHz)\n", f_PUSCH);
    printf("\n");

    for (i=0; i<symbols[1]; i++)
    {
        (i > 9) ? printf("[ #%d]", i) : printf("[ #%d ]", i);
    }
    printf("\n");
    printf("[[1;35mCP[0m]");
    for (i=0; i<(N_u / 2048); i++)
    {
        printf("[[1;33mSEQ[0m]");
    }
    if (N_gap > 0)
    {
        printf("[[1;36mGP[0m]");
    }
    printf("\n");

    printf("\n");
    printf("        %.2f KHz\n", (kb * f_PRACH));
    printf("          v\n");
    printf("Preamble   |<---- %.2f KHz ---->|\n", subcarriers[0]);
    printf("     RBs |<------ %.2f KHz ------>|\n", subcarriers[1]);
    printf("\n");

    return 0;
}

