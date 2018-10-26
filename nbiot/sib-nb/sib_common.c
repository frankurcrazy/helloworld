#include <stdio.h>
#include "sib_common.h"


/* 36.214 Table 16.4.1.3-3 */
static uint16  _sib1NbRepNum[16] = {
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   0, 0, 0, 0
};

/* 36.214 Table 16.4.1.5.2-1 */
static uint16  _sib1NbTbSize[16] = {
   208, 208, 208,
   328, 328, 328,
   440, 440, 440,
   680, 680, 680,
   0, 0, 0, 0
};

/* 36.214 Table 16.4.1.3-4 */
static int  _sib1NbStartFrm[16][4] = {
   {  0, 16, 32, 48  },  //0
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //3
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //6
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //9
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   { -1, -1, -1, -1  },  //12
   { -1, -1, -1, -1  },
   { -1, -1, -1, -1  },
   { -1, -1, -1, -1  }
};


/* SIBx-NB available mask */
uint16  g_sib1Subframe[1024];
uint16  g_sib2Subframe[1024];

/* SIBx-NB repetition subframe */
uint16  g_sib1Repetition[1024];
uint16  g_sib2Repetition[1024];


void sib1Nb_schedule(
    int  cellId,
    int  schInfo,
    int *pRepNum,
    int *pTbSize,
    int *pStartFrm
)
{
    int cellIdLsb;
    int repNum;
    int tbSize;
    int startFrm;

    uint16 frame = 0;
    int i;
    int j;
    int k;


    /* g_sib1Subframe[] example:
     *
     *    |  0  1  2  3  4  5  6  7  8  9  
     * ---+--------------------------------
     *  0 |  x           S  x           x
     *  1 |  x              x
     *  2 |  x           S  x           x
     *  3 |  x              x
     *  4 |  x           S  x           x
     *  5 |  x              x
     *  6 |  x           S  x           x
     *  7 |  x              x
     *  8 |  x           S  x           x
     *  9 |  x              x
     * 10 |  x           S  x           x
     * 11 |  x              x
     * 12 |  x           S  x           x
     * 13 |  x              x
     * 14 |  x           S  x           x
     * 15 |  x              x
     */
    cellIdLsb = (cellId & 0x3);
    repNum = _sib1NbRepNum[ schInfo ];
    tbSize = _sib1NbTbSize[ schInfo ];
    startFrm = _sib1NbStartFrm[ schInfo ][ cellIdLsb ];

    if (4 == repNum)
    {
        for (k=0; k<4; k++)
        {
            /* 256 frames period */
            for (j=0; j<16; j++)
            {
                /* repetition 64 frames */
                for (i=0; i<16; i++)
                {
                    /* scheduling for 8 subframes (N_SF) */
                    if ((cellIdLsb == (j & 0x3)) && (0 == (i & 0x1)))
                    {
                        g_sib1Subframe[frame] = 0x010;
                    }
                    else
                    {
                        g_sib1Subframe[frame] = 0;
                    }
                    g_sib1Repetition[frame] = (j != cellIdLsb);
                    frame++;
                }
            }
        }
    }
    else if (8 == repNum)
    {
        for (k=0; k<4; k++)
        {
            /* 256 frames period */
            for (j=0; j<16; j++)
            {
                /* repetition 128 frames */
                for (i=0; i<16; i++)
                {
                    /* scheduling for 8 subframes (N_SF) */
                    if ((cellIdLsb == (j & 0x1)) && (0 == (i & 0x1)))
                    {
                        g_sib1Subframe[frame] = 0x010;
                    }
                    else
                    {
                        g_sib1Subframe[frame] = 0;
                    }
                    g_sib1Repetition[frame] = (j != cellIdLsb);
                    frame++;
                }
            }
        }
    }
    else
    {
        for (i=0; i<1024; i++)
        {
            /* repetition every frame and scheduling for 8 subframes (N_SF) */
            if (cellIdLsb == (i & 0x1))
            {
                g_sib1Subframe[frame] = 0x010;
            }
            else
            {
                g_sib1Subframe[frame] = 0;
            }
            g_sib1Repetition[frame] = ((i & 0xFF) > 15);
            frame++;
        }
    }

    if ( pRepNum )   (*pRepNum) = repNum;
    if ( pTbSize )   (*pTbSize) = tbSize;
    if ( pStartFrm ) (*pStartFrm) = startFrm;
}

void sib2Nb_schedule(int period, int repetition, int window)
{
    int n_abs_sf;
    int p_index;
    int w_index;
    int r_index;
    int r_count;
    int sib2_nsf;
    int sib2_state;  /* 0: ready, 1: transmitted, 2: wait */

    uint16 mask;
    int i;
    int j;


    /* g_sib2Subframe[] example:
     *
     *    |  0  1  2  3  4  5  6  7  8  9  
     * ---+--------------------------------
     *  0 |  x  S  S  S  x  x  S  S  S  x
     *  1 |  x  S  S        x
     *  2 |  x           x  x           x
     *  3 |  x              x
     *  4 |  x  S  S  S  x  x  S  S  S  x
     *  5 |  x  S  S        x
     *  6 |  x           x  x           x
     *  7 |  x              x
     *  8 |  x  S  S  S  x  x  S  S  S  x
     *  9 |  x  S  S        x
     * 10 |  x           x  x           x
     * 11 |  x              x
     * 12 |  x  S  S  S  x  x  S  S  S  x
     * 13 |  x  S  S        x
     * 14 |  x           x  x           x
     * 15 |  x              x
     */
    r_count = 0;
    sib2_nsf = 0;
    sib2_state = 0;

    for (i=0; i<1024; i++)
    {
        g_sib2Subframe[i] = 0;
        g_sib2Repetition[i] = 0;

        mask  = g_sib1Subframe[i];
        mask |= ((0 == (i & 0x1)) ? 0x221 : 0x021);

        for (j=0; j<10; j++)
        {
            n_abs_sf = ((i * 10) + j);
            p_index = (n_abs_sf % (period * 10));
            if (0 == p_index)
            {
                sib2_state = 0;
            }
            w_index = (n_abs_sf % window);
            if (0 == w_index)
            {
                /* start of SI window */
                sib2_nsf = 0;
                r_count = 0;
                if (0 == sib2_state)
                {
                    sib2_state = 1;
                }
                else
                {
                    sib2_state = 2;
                }
            }
            r_index = (w_index % (repetition * 10));
            if (0 == r_index)
            {
                /* start of SIB2 transmission */
                sib2_nsf = 0;
                r_count++;
            }

            if (0 == (mask & (0x1 << j)))
            {
                if ((sib2_nsf < 8) && (1 == sib2_state))
                {
                    g_sib2Subframe[i] |= (0x1 << j);
                    if (r_count > 1)
                    {
                        g_sib2Repetition[i] |= (0x1 << j);
                    }
                }

                sib2_nsf++;
            }
        }
    }
}

