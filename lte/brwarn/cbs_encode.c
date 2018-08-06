#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gsm7bit.h"


static uint16 _encode7bitAlphabet(
    uint8  *pOut,
    uint16  outSizeMax,
    uint8  *pIn,
    uint16  inSize
)
{
    uint8   mask[] = {
                0x80,  // 0
                0xC0,  // 1
                0xE0,  // 2
                0xF0,  // 3
                0xF8,  // 4
                0xFC,  // 5
                0xFE   // 6
            };
    uint8  *pOutBuf = (pOut + 1);
    uint8  *pInBuf  = NULL;
    uint8  *pGsmBuf = NULL;
    uint16  outSize = 0;
    uint16  msgSize;
    uint16  packSize;
    uint16  pageSize;
    uint8   byte;
    int n;
    int m;
    int i;
    int j;
    int k;


    if (0 == inSize)
    {
        printf("GSM 7-bit alphabet length is 0\n");
        return 0;
    }

    /* convert ASCII to GSM 7-bit character */
    pGsmBuf = malloc(inSize * 2);
    if (NULL == pGsmBuf)
    {
        printf("%s: out of memory\n", __func__);
        return 0;
    }
    inSize = asciiToGsm7bit(pIn, inSize, pGsmBuf, (inSize * 2));
    pInBuf = pGsmBuf;

    /* TS 23.041 9.3.19 CBS-Message-Information-Page */
    n = CEIL_DIV(inSize, 93);
    if (n > 13)
    {
        printf("GSM 7-bit alphabet length(%u) is too large\n", inSize);
        n = 13;
    }

    pOut[0] = n;
    msgSize = inSize;
    outSize = (1 + (n * 83));

    /* TS 23.038 6.2 Character sets and coding */
    for (i=0; i<n; i++)
    {
        if (msgSize >= 93)
        {
            pageSize  = 82;
            packSize  = 93;
            msgSize  -= 93;
            memset(pOutBuf, 0x00, 82);
        }
        else
        {
            pageSize = CEIL_DIV((msgSize * 7), 8);
            packSize = msgSize;
            for (j=0; j<82; j++)
            {
                m = (j % 7);
                if (j == 81)
                {
                    byte = ((0x0d) >> m);
                }
                else
                {
                    byte = (((0x0d) >> m) |
                            ((0x0d << (7 - m)) & mask[m]));
                }
                pOutBuf[j] = byte;
            }
        }

        /*
        *         0 1 2 3 4 5 6 0  1 2  3  4  5  6  0  1   2  3  4  5  6  0  1  2   3
        *  in[k]  0 1 2 3 4 5 6,7  8 9 10 11 12 13 14,15  16 17 18 19 20 21 22,23  24
        *
        *  out[j] 0 1 2 3 4 5 6    7 8  9 10 11 12 13     14 15 16 17 18 19 20     21
        *  mask   0 1 2 3 4 5 6    0 1  2  3  4  5  6      0  1  2  3  4  5  6      0
        */
        for (j=0, k=0; j<82; j++)
        {
            m = (j % 7);
            if (k == (packSize - 1))
            {
                byte = (((pInBuf[k] & 0x7F) >> m) |
                        (pOutBuf[j] & mask[m]));
            }
            else
            {
                byte = (((pInBuf[k] & 0x7F) >> m) |
                        ((pInBuf[k + 1] << (7 - m)) & mask[m]));
            }
            pOutBuf[j] = byte;

            if (m == 6) k += 2;
            else        k ++;

            if (k >= packSize)
            {
                if (pageSize != (j + 1))
                {
                    printf("wrong calculation %d <--> %d\n", pageSize, (j + 1));
                    pageSize = (j + 1);
                }
                break;
            }
        }

        pOutBuf[82] = pageSize;
        pOutBuf += 83;
        pInBuf  += packSize;
    }

    free( pGsmBuf );

    return outSize;
}

static uint16 _encode8bitData(
    uint8  *pOut,
    uint16  outSizeMax,
    uint8  *pIn,
    uint16  inSize
)
{
    uint8  *pOutBuf = (pOut + 1);
    uint8  *pInBuf  = pIn;
    uint16  outSize = 0;
    uint16  msgSize;
    uint16  pageSize;
    int n;
    int i;


    if (0 == inSize)
    {
        printf("8-bit data length is 0\n");
        return 0;
    }

    /* TS 23.041 9.3.19 CBS-Message-Information-Page */
    n = CEIL_DIV(inSize, 82);
    if (n > 15)
    {
        printf("8-bit data length(%u) is too large\n", inSize);
        n = 15;
    }

    pOut[0] = n;
    msgSize = inSize;
    outSize = (1 + (n * 83));

    /* TS 23.038 6.2 Character sets and coding */
    for (i=0; i<n; i++)
    {
        if (msgSize >= 82)
        {
            pageSize  = 82;
            msgSize  -= 82;
        }
        else
        {
            pageSize = msgSize;
            memset(pOutBuf, 0x0d, 82);
        }

        memcpy(pOutBuf, pInBuf, pageSize);

        pOutBuf[82] = pageSize;
        pOutBuf += 83;
        pInBuf  += pageSize;
    }

    return outSize;
}

static uint16 _encode16bitUcs2(
    uint8  *pOut,
    uint16  outSizeMax,
    uint8  *pIn,
    uint16  inSize
)
{
    uint8  *pOutBuf = (pOut + 1);
    uint8  *pInBuf  = (pIn  + 2);
    uint16  outSize = 0;
    uint16  msgSize = (inSize - 2);
    uint16  pageSize;
    uint8   language[2];
    int n;
    int i;
    int j;


    if (0 == inSize)
    {
        printf("UCS2 message length is 0\n");
        return 0;
    }

    /* Get language type */
    language[0] = pIn[0];
    language[1] = pIn[1];

    /* TS 23.041 9.3.19 CBS-Message-Information-Page */
    /*
    *    1   2    80         1
    *  +---+----+----------+---+
    *  | n | xx |   page   | s |
    *  +---+----+----------+---+
    *      | xx |   page   | s |
    *      +----+----------+---+
    *      |      ...      | s |
    *      +----+----------+---+
    *      | xx |   page   | s |
    *      +----+----------+---+
    */
    n = CEIL_DIV(inSize, 80);
    if (n > 15)
    {
        printf("UCS2 message length(%u) is too large\n", inSize);
        n = 15;
    }

    pOut[0] = n;
    outSize = (1 + (n * 83));

    /* TS 23.038 6.2 Character sets and coding */
    for (i=0; i<n; i++)
    {
        if (msgSize >= 80)
        {
            pageSize  = 80;
            msgSize  -= 80;
        }
        else
        {
            pageSize = msgSize;
            for (j=0; j<82; j++)
            {
                pOutBuf[j] = (((j % 2) == 0) ? 0x00 : 0x0d);
            }
        }

        /* Set language */
        memcpy(pOutBuf, language, 2);
        pOutBuf += 2;
        /* Set page context */
        memcpy(pOutBuf, pInBuf, pageSize);
        pOutBuf += 80;
        /* Set page size */
        pOutBuf[0] = pageSize + 2;
        pOutBuf ++;

        pInBuf  += pageSize;
    }

    return outSize;
}


#if 0
uint8   _codingScheme = 0xF4;
uint8   _pWarnString[] = "Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test string. Alifecom warning test stringX";
uint16  _warnStringLen = sizeof( _pWarnString );
#endif
#if 0
uint8   _codingScheme = 0xF4;
uint8   _pWarnString[] = "0123456789.X";
uint16  _warnStringLen = sizeof( _pWarnString );
#endif
#if 1
uint8   _codingScheme = 0x01;
uint8   _pWarnString[] = "Svc[123";
uint16  _warnStringLen = (sizeof( _pWarnString ) - 1);
#endif
#if 0
uint8   _codingScheme = 0x01;
uint8   _pWarnString[] = "123{}[]\\|~1234";
uint16  _warnStringLen = (sizeof( _pWarnString ) - 1);
#endif
#if 0
uint8   _codingScheme = 0x01;
uint8   _pWarnString[] = "abcabcabcabcX";
uint16  _warnStringLen = (sizeof( _pWarnString ) - 1);
#endif
#if 0
uint8   _codingScheme = 0x01;
uint8   _pWarnString[] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz~~~";
uint16  _warnStringLen = (sizeof( _pWarnString ) - 1);
#endif
#if 0
uint8   _codingScheme = 0x11;
uint8   _pWarnString[] = {
    0x7a, 0x34,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5
};
uint16  _warnStringLen = sizeof( _pWarnString );
#endif
#if 0
uint8   _codingScheme = 0x11;
uint8   _pWarnString[] = {
    0x7a, 0x34,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5, 0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5, 0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5, 0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5, 0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5,
    0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5, 0xb5, 0xd8, 0xb3, 0xfc, 0xb4, 0xfa, 0xb8, 0xd5
};
uint16  _warnStringLen = sizeof( _pWarnString );
#endif

int main(void)
{
    uint16  warnContentLen = 0;
    uint8   warnContent[1250];

    printf("String length = %u\n", _warnStringLen);
    memset(warnContent, 0x00, 1250);

    switch ( _codingScheme )
    {
        /* GSM 7-bit default alphabet */
        case 0x01:
        case 0x40:
            warnContentLen = _encode7bitAlphabet(
                                 warnContent,
                                 1250,
                                 _pWarnString,
                                 _warnStringLen
                             );
            break;

        /* UCS2 16-bit */
        case 0x11:
        case 0x48:
            warnContentLen = _encode16bitUcs2(
                                 warnContent,
                                 1250,
                                 _pWarnString,
                                 _warnStringLen
                             );
            break;

        /* 8-bit data */
        case 0x44:
        case 0xF4:
        default:
            warnContentLen = _encode8bitData(
                                 warnContent,
                                 1250,
                                 _pWarnString,
                                 _warnStringLen
                             );
    }

    mem_dump(warnContent, warnContentLen);
    return 0;
}

