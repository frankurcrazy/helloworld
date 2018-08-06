#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* Number of symbols per UL slot */
#define SYMBOLS_PER_UL_SLOT     (7)

/* Used PN sequence length in 36.211, 5.4 */
#define PUCCH_PN_SEQ_LEN        (1120)

/* Number of slots per frame */
#define SLOTS_PER_FRAME         (20)

/* Number of subcarriers per UL RB */
#define SUBCARRIERS_PER_UL_RB   (12)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef struct _tPucchPhyParam
{
    /* n-cs(ns,l) for even slot */
    uint8   nCs[SYMBOLS_PER_UL_SLOT];

    /* n'(ns) * delta-PUCCH-shift for even slot */
    uint16  nPrimeByDeltaShift;

    /* n-oc(ns) for even slot */
    uint8   nOc;

    /* m parameter */
    uint8   m;
} tPucchParam;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

/* c parameter for PUCCH parameters */
static   uint8   _cParamForPucch = 3;

/* N-1-CS parameter for PUCCH parameters */
static   uint8   _n1Cs = 0;

/* Delta-PUCCH-shift parameter for PUCCH parameters */
static   uint8   _deltaPucchShift = 1;

/* (c * N-1-CS) parameter */
static   uint8   _cByN1Cs;

/* (c * N-RB-SC) parameter */
static   uint8   _cByRbSc;

/* nRB-CQI (abbreviated as N-2-RB in PHY spec) paramter */
static   uint8   _nRbCqi;

/* c-cell-cs(ns,l) for each ns and l */
static   uint8   _cCellCs[SLOTS_PER_FRAME][SYMBOLS_PER_UL_SLOT];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

static uint32 _shiftX1Seq(uint32 x1, uint32 shiftSize)
{
    uint32  shiftedX1 = x1;
    uint32  leftShiftSize = shiftSize;
    uint32  newBit = 0;

    /* While there is still shifting to be made, make 1 bit shift */
    while (leftShiftSize > 0)
    {
        /* Shift X1 sequence by 1 */
        shiftedX1 = shiftedX1 >> 1;

        /* Calculate new bit : x1(31) = (x1(3) + x1(0)) mod 2 */
        newBit = (((shiftedX1 >> 3) & 0x01) + (shiftedX1 & 0x01)) % 2;

        /* Move new bit into X1 sequence */
        shiftedX1 |= (newBit << 31);

        /* Decrement shift size left to be made by 1 */
        leftShiftSize -= 1;
    }

    return shiftedX1;
}

static uint32 _shiftX2Seq(uint32 x2, uint32 shiftSize)
{
    uint32  shiftedX2 = x2;
    uint32  leftShiftSize = shiftSize;
    uint32  newBit = 0;

    /* While there is still shifting to be made, make 1 bit shift */
    while (leftShiftSize > 0)
    {
        /* Shift X2 sequence by 1 */
        shiftedX2 = shiftedX2 >> 1;

        /* Calculate new bit : X2(31) = (X2(3) + X2(2) + X2(1) + X2(0)) mod 2 */
        newBit =
            (
                ((shiftedX2 >> 3) & 0x01) +
                ((shiftedX2 >> 2) & 0x01) +
                ((shiftedX2 >> 1) & 0x01) +
                (shiftedX2 & 0x01)
            ) % 2;

        /* Move new bit into X2 sequence */
        shiftedX2 |= (newBit << 31);

        /* Decrement shift size left to be made by 1 */
        leftShiftSize -= 1;
    }

    return shiftedX2;
}

static void _genPnSequence(uint32 cinit, uint32 size, uint32 *pSeq)
{
    uint32  x1 = 0x80000001;
    uint32  x2 = cinit;
    uint32  neededArraySize = DIV_CEIL(size, 32);
    uint32  seqArrayIndex = 0;

    /* Calculate x2(31) = (x2(0) + x2(1) + x2(2) + x2(3)) mod 2 */
    x2 |=
        (
            (
                (x2 & 0x01) +
                ((x2 >> 1) & 0x01) +
                ((x2 >> 2) & 0x01) +
                ((x2 >> 3) & 0x01)
            ) % 2
        ) << 31;

    /* Shift X1 sequence and X2 sequence to index 1600 */
    x1 = _shiftX1Seq(x1, 1600);
    x2 = _shiftX2Seq(x2, 1600);

    /*
    *  For needed PN sequence, calculate its value by
    *  c(n) = (x1(n + 1600) + x2(n + 1600)) mod 2.
    *  Actually the above formula could be simplified as
    *  c(n) = x1(n + 1600) ^ x2(n + 1600)
    */
    for (seqArrayIndex = 0; seqArrayIndex < neededArraySize; seqArrayIndex++)
    {
        /* Calculate 32 bits PN sequence in this run */
        pSeq[seqArrayIndex] = x1 ^ x2;

        /*
        *  If there are still PN sequence to be calculated, prepare for
        *  next run
        */
        if (seqArrayIndex != neededArraySize - 1)
        {
            /* Prepare X1 and X2 sequence for next 32 bits */
            x1 = _shiftX1Seq(x1, 32);
            x2 = _shiftX2Seq(x2, 32);
        }
    }
}

/**
 *  Get PN sequence value from a PN sequence.
 *  @param [in]  pSeq - PN sequence.
 *  @param [in]  index - Index of PN sequence to get.
 *  @returns Value of PN sequence.
 */    
static uint8 _getPnSeqValue(uint32 *pSeq, uint32 index)
{
    uint8   pnValue = 0;

    /* Get PN sequence value from the PN sequence via index */
    pnValue = (pSeq[index / 32] >> (index % 32)) & 0x01;

    return pnValue;
}

/**
 *  Initialize PUCCH parameters calculation.
 *  @param [in]  pci - PHY cell ID.
 *  @param [in]  n1Cs - N-1-CS (0 ~ 7).
 *  @param [in]  deltaShift - delta-PUCCH-shift (1 ~ 3).
 *  @param [in]  nRbCqi - N_CQI RB (0 ~ 98).
 *  @param [in]  cp - cyclic prefix (0 : normal, 1 : extended).
 *  @returns void.
 */
void init_pucchParams(
    uint16  pci,
    uint8   n1Cs,
    uint8   deltaShift,
    uint8   nRbCqi,
    uint8   cp
)
{
    static uint32  pciPnSeq[DIV_CEIL(PUCCH_PN_SEQ_LEN, 32)];
    uint8          ns;
    uint8          l;
    uint8          bitIndex;
    uint32         pnSeqIndex;

    /* Set c parameter according to CP type */
    if (cp == 0)
    {
        _cParamForPucch = 3;
    }
    else
    {
        _cParamForPucch = 2;
    }

    /* Set N-1-CS parameter */
    _n1Cs = n1Cs;

    /* Set Delta-PUCCH-shift parameter */
    _deltaPucchShift = deltaShift;

    /* Calculate (c * N-1-CS) parameter */
    _cByN1Cs = _cParamForPucch * _n1Cs;

    /* Calculate (c * N-RB-SC) parameter */
    _cByRbSc = _cParamForPucch * SUBCARRIERS_PER_UL_RB;

    /* Set nRB-CQI parameter */
    _nRbCqi = nRbCqi;

    /*
    *  Generate needed PN sequence for c-cell-cs(ns, l) that is initialized by
    *  PCI
    */
    _genPnSequence(pci, PUCCH_PN_SEQ_LEN, pciPnSeq);

    /* Calculate c-cell-cs(ns, l) for every ns and l */
    for (ns = 0; ns < SLOTS_PER_FRAME; ns++)
    {
        for (l = 0; l < SYMBOLS_PER_UL_SLOT; l++)
        {
            _cCellCs[ns][l] = 0;
            for (bitIndex = 0; bitIndex < 8; bitIndex++)
            {
                pnSeqIndex = 8 * SYMBOLS_PER_UL_SLOT * ns + 8 * l + bitIndex;
                _cCellCs[ns][l] |=
                    _getPnSeqValue(pciPnSeq, pnSeqIndex) << bitIndex;
            }
        }

        #if 0
        //if ((ns % 2) == 0)
        {
            printf(
                "%2d : %3d %3d %3d %3d %3d %3d %3d\n",
                ns,
                _cCellCs[ns][0],
                _cCellCs[ns][1],
                _cCellCs[ns][2],
                _cCellCs[ns][3],
                _cCellCs[ns][4],
                _cCellCs[ns][5],
                _cCellCs[ns][6]
            );
        }
        #endif
    }
}

void calc_pucchParam(
    uint16       n1Pucch,
    uint8        subfrm,
    tPucchParam *pParam
)
{
    uint8   ns = 2 * subfrm;
    uint8   upperNPrime;
    uint16  n1PucchByDeltaShift = n1Pucch * _deltaPucchShift;
    uint8   l;

    #if 0
    printf("DEBUG>\n");
    printf("DEBUG> n1PucchByDeltaShift : %d\n", n1PucchByDeltaShift);
    printf("DEBUG> _cByN1Cs            : %d\n", _cByN1Cs);
    printf("DEBUG>\n\n");
    #endif

    /* Determine N' value */
    if (n1PucchByDeltaShift < _cByN1Cs)
    {
        upperNPrime = _n1Cs;
    }
    else
    {
        upperNPrime = SUBCARRIERS_PER_UL_RB;
    }

    /* Calculate n' * delta-PUCCH-shift value on even slot */
    if (n1PucchByDeltaShift < _cByN1Cs)
    {
        pParam->nPrimeByDeltaShift = n1PucchByDeltaShift;
    }
    else
    {
        pParam->nPrimeByDeltaShift =
            (n1PucchByDeltaShift - _cByN1Cs) % _cByRbSc;
    }

    /* Calculate n-oc on even slot */
    pParam->nOc = DIV_FLOOR(pParam->nPrimeByDeltaShift, upperNPrime);
    if (_cParamForPucch == 2)
    {
        pParam->nOc = 2 * pParam->nOc;
    }

    /* Calculate n-cs for every symbol on even slot */
    for (l = 0; l < SYMBOLS_PER_UL_SLOT; l++)
    {
        if (_cParamForPucch == 3)
        {
            pParam->nCs[l] =
                (
                    _cCellCs[ns][l] +
                    (
                        pParam->nPrimeByDeltaShift +
                        pParam->nOc % _deltaPucchShift
                    ) % upperNPrime
                )  % SUBCARRIERS_PER_UL_RB;
        }
        else
        {
            pParam->nCs[l] =
                (
                    _cCellCs[ns][l] +
                    (
                        pParam->nPrimeByDeltaShift +
                        pParam->nOc / 2
                    ) % upperNPrime
                )  % SUBCARRIERS_PER_UL_RB;
        }
    }

    /* Calculate m */
    if (n1PucchByDeltaShift < _cByN1Cs)
    {
        pParam->m = _nRbCqi;
    }
    else
    {
        pParam->m =
            DIV_FLOOR(n1PucchByDeltaShift - _cByN1Cs, _cByRbSc) +
            _nRbCqi +
            DIV_CEIL(_n1Cs, 8);
    }
}


int main(int argc, char *argv[])
{
    tPucchParam  pucchParam;
    uint16  cellId;
    uint8   pucch_nCsAn;
    uint8   pucch_deltaShift;
    uint8   pucch_nRbCqi;
    uint8   cp;
    uint16  n1Pucch;
    uint8   subfrmNo;

    if (1 == argc)
    {
        printf("Usage: pucch_param <subframe_number> <pucch_index>\n\n");
        return -1;
    }

    subfrmNo = (uint8)atoi( argv[1] );
    n1Pucch = (uint16)atoi( argv[2] );

    cellId = 0;
    pucch_nCsAn = 0;
    pucch_nRbCqi = 2;
    pucch_deltaShift = 1;
    cp = 0;

    init_pucchParams(
        cellId,
        pucch_nCsAn,
        pucch_deltaShift,
        pucch_nRbCqi,
        cp
    );

    /* fill the 2nd part of PUCCH index */
    calc_pucchParam(
        n1Pucch,
        subfrmNo,
        &pucchParam
    );

    printf("cellId           = %d\n", cellId);
    printf("pucch_nCsAn      = %d\n", pucch_nCsAn);
    printf("pucch_nRbCqi     = %d\n", pucch_nRbCqi);
    printf("pucch_deltaShift = %d\n", pucch_deltaShift);
    printf("cp               = %d\n", cp);
    printf("pucchIndex = %d\n", n1Pucch);
    printf("surframeNo = %d\n", subfrmNo);
    printf("\n");
    printf("nCs[0] = %d\n", pucchParam.nCs[0]);
    printf("nCs[1] = %d\n", pucchParam.nCs[1]);
    printf("nCs[2] = %d\n", pucchParam.nCs[2]);
    printf("nCs[3] = %d\n", pucchParam.nCs[3]);
    printf("nCs[4] = %d\n", pucchParam.nCs[4]);
    printf("nCs[5] = %d\n", pucchParam.nCs[5]);
    printf("nCs[6] = %d\n", pucchParam.nCs[6]);
    printf("nOc    = %d\n", pucchParam.nOc   );
    printf("nPrimeByDeltaShift = %d\n",pucchParam.nPrimeByDeltaShift);
    printf("m = %d\n", pucchParam.m);
    printf("\n");


    return 0;
}

