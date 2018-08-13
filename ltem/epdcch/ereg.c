#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int RE[12][14] = {{0}};
int DMRS[12][14] = {
  /* 0  1  2  3  4  5  6   7  8  9 10 11 12 13 */
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }, //  0
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }, //  1
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  2
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  3
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  4
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }, //  5
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }, //  6
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  7
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  8
  {  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0  }, //  9
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }, // 10
  {  0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 1, 1  }  // 11
};

int N_ECCE_EREG_4[4][4] = {
  {  0,  4,  8, 12 },
  {  1,  5,  9, 13 },
  {  2,  6, 10, 14 },
  {  3,  7, 11, 15 }
};

int N_ECCE_EREG_8[2][8] = {
  {  0,  2,  4,  6,  8, 10, 12, 14 },
  {  1,  3,  5,  7,  9, 11, 13, 15 }
};


int ECCEindex(int N_ECCE_EREG, int EREG)
{
    int ECCE;
    int i;

    if (4 == N_ECCE_EREG)
    {
        for (ECCE=0; ECCE<4; ECCE++)
        {
            for (i=0; i<4; i++)
            {
                if (EREG == N_ECCE_EREG_4[ECCE][i])
                {
                    return ECCE;
                }
            }
        }
    }
    else if (8 == N_ECCE_EREG)
    {
        for (ECCE=0; ECCE<2; ECCE++)
        {
            for (i=0; i<8; i++)
            {
                if (EREG == N_ECCE_EREG_8[ECCE][i])
                {
                    return ECCE;
                }
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    int N_ECCE_EREG = 4;
    int EREG;
    int k;
    int l;


    if (argc > 1)
    {
        N_ECCE_EREG = atoi( argv[1] );
    }


    EREG = 0;
    for (l=0; l<14; l++)
    {
        for (k=0; k<12; k++)
        {
            if ( DMRS[k][l] )
            {
                RE[k][l] = -1;
            }
            else
            {
                RE[k][l] = EREG;
                EREG = (EREG + 1) & 0xF;
            }
        }
    }

    for (k=11; k>=0; k--)
    {
        for (l=0; l<14; l++)
        {
            int color = ECCEindex(N_ECCE_EREG, RE[k][l]);

            switch ( color )
            {
                case 0:
                    printf("[30;41m");
                    break;
                case 1:
                    printf("[30;43m");
                    break;
                case 2:
                    printf("[30;46m");
                    break;
                case 3:
                    printf("[30;47m");
                    break;
                default:
                    ;
            }
            printf(" %3d", RE[k][l]);
            printf("[0m");
        }
        printf("\n");
    }


    printf("\n");

    return 0;
}
