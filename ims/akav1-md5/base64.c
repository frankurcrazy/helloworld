#include "base64.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define B64_DEF_LINE_SIZE  (76)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

/*
* Translation Table as described in RFC1113
*/
static const char cb64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
* Translation Table to decode (created by author)
*/
static const char cd64[] =
    "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/*
* encodeblock
*
* encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void encodeblock(unsigned char in[3], unsigned char out[4], int len)
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*
* b64_encode
*
* base64 encode a stream adding padding and line breaks as per spec.
*/
unsigned int b64_encode(
    unsigned char *pInput,
    unsigned int   inputLen,
    char          *pOutput,
    unsigned int   outputLen
)
{
    unsigned char in[3], out[4];
    int i, len, blocksout = 0;
    unsigned int _rc=0;
    unsigned char *pHead = pInput;
    unsigned char *pTail = pInput + inputLen;
    char *pBuf  = pOutput;

    //swchen
    if((pInput==NULL) || (pOutput==NULL) /*||(inputLen==0) ||(outputLen==0)*/)
    {
        _rc = outputLen+2;
        goto _OUT;
    }

    if( outputLen < b64_encBufLen(inputLen))
    {
        printf(
            "Warning!! ***** Please check output buffer length!! *****(%s)\n",
            __FUNCTION__
        );

        _rc = outputLen+3;
        goto _OUT;
    }

    while (pHead < pTail)
    {
        len = 0;
        for (i=0; i<3; i++)
        {
            in[i] = *pHead;
            if (pHead < pTail)
            {
                len++;
            }
            else
            {
                in[i] = 0;
            }
            pHead++;
        }

        if ( len )
        {
            encodeblock(in, out, len);
            for(i=0; i<4; i++)
            {
                *pBuf = out[i];
                pBuf++;
            }
            blocksout++;
        }

        if ( (blocksout >= (B64_DEF_LINE_SIZE/4)) )
        {
            if ( blocksout )
            {
                *pBuf = '\n';
                pBuf++;
            }
            blocksout = 0;
        }
    }

    *pBuf = 0x00;

    if( (pBuf - pOutput) > outputLen )
    { 
        _rc = outputLen+1;
    }
    else
    {
        _rc = pBuf - pOutput;
    }

_OUT:
   return _rc;
}

/*
* decodeblock
*
* decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void decodeblock( unsigned char in[4], unsigned char out[3] )
{   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/*
* b64_decode
*
* decode a base64 encoded stream discarding padding, line breaks and noise
*/
unsigned int b64_decode(
    char          *pInput,
    unsigned int   inputLen,
    unsigned char *pOutput,
    unsigned int   outputLen
)
{
    unsigned char in[4], out[3], v;
    int i, len;

    char *pHead = pInput;
    char *pTail = pInput + strlen(pInput) + 1;
    unsigned char *pBuf  = pOutput;
    unsigned int _rc=0;

    if((pInput==NULL) || (pOutput==NULL))
    {
        _rc = outputLen+2;
        goto _OUT;
    }

    while (pHead < pTail)
    {
        for (len=0, i=0; (i < 4) && (pHead < pTail); i++)
        {
            v = 0;
            while ((pHead < pTail) && (v == 0))
            {
                v = (unsigned char)(*pHead);
                v = (unsigned char)((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
                pHead++;
            }

            if (pHead < pTail)
            {
                len++;
                if ( v )
                {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else
            {
                in[i] = 0;
            }
        }
        if ( len )
        {
            decodeblock(in, out);

            for(i=0; i<(len-1); i++)
            {
                if ((pBuf-pOutput) >= outputLen)
                {
                    printf(
                        "Warning!! ***** Please check output buffer length!! *****(%s)\n",
                        __FUNCTION__
                    );

                    _rc = outputLen+3;
                    break;
                }

                *pBuf = out[i];
                pBuf++;
            }
        }
    }

    if((pBuf - pOutput) > outputLen)
    {
        _rc = outputLen+1;
    }
    else 
    { 
        _rc = pBuf - pOutput;
    }

_OUT:
   return _rc;
}


unsigned int b64_encBufLen(unsigned int binSize)
{
    unsigned int strSize;
    unsigned int padSize;

    padSize = (((binSize % 3) > 0) ? 4 : 0);
    strSize = (binSize / 3) * 4 + padSize;

    if (strSize > B64_DEF_LINE_SIZE)
    {
        strSize += ((strSize / B64_DEF_LINE_SIZE) + 1);
    }

    strSize++;  // include the end of string

    return strSize;
}

unsigned int b64_decBufLen(unsigned int strSize)
{
    unsigned int binSize;

    binSize = (strSize / 4) * 3;
    //binSize -= padSize; /* number of '=' */

    return binSize;
}

