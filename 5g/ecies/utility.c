#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void mem_dump(void *pAddr, int len)
{
    unsigned char *pByte = (unsigned char *)pAddr;
    int i;

    if (pAddr == NULL)
    {
        printf("NULL\n\n");
        return;
    }

    for(i=0; i<len; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf(" %02x", pByte[i]);
    }
    printf("\n");
    printf(" (%u bytes)\n", len);
}

void hex_dump(char *pName, unsigned char *pByte, int len)
{
    int i;

    printf("[1;33m%s[0m:\n", pName);
    if ( pByte )
    {
        for (i=0; i<len; i++)
        {
            printf("%02x", pByte[i]);
        }
        printf("\n");
    }
    printf("\n");
}

static uint8 _hex2dec(char ch)
{
    uint8 val = 0;

    switch ( ch )
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            val = (ch - '0');
            break;
        case 'a':
        case 'A':
            val = 10;
            break;
        case 'b':
        case 'B':
            val = 11;
            break;
        case 'c':
        case 'C':
            val = 12;
            break;
        case 'd':
        case 'D':
            val = 13;
            break;
        case 'e':
        case 'E':
            val = 14;
            break;
        case 'f':
        case 'F':
            val = 15;
            break;
        default:
            ;
    }

    return val;
}

int hex_string_to_byte_array(
    char   *pString,
    uint16  stringLen,
    uint8  *pData,
    uint16  dataLen
)
{
    uint8  nibbleH;
    uint8  nibbleL;
    uint8  byte;
    int i;
    int j;

    if ((stringLen % 2) != 0)
    {
        printf(
            "%s: wrong input string length %d\n",
            __func__,
            stringLen
        );
        return -1;
    }

    if (dataLen < (stringLen >> 1))
    {
        printf(
            "%s: un-enough output buffer length %d\n",
            __func__,
            dataLen
        );
        return -1;
    }

    /*
    * Example:
    *   Input   "11223344"
    *   Output  0x11 0x22 0x33 0x44
    */
    for (i=0, j=0; i<stringLen; i+=2)
    {
        nibbleH = _hex2dec( pString[i  ] );
        nibbleL = _hex2dec( pString[i+1] );

        byte = ((nibbleH << 4) | (nibbleL));

        pData[j++] = byte;
    }

    return 0;
}

