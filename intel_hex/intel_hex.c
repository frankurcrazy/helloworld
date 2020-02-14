#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "intel_hex.h"


#define APP_NAME   "intel_hex"
#if 1
#define EMPTY_BYTE 0xFF
#else
#define EMPTY_BYTE 0x00
#endif
#define TEXT_SIZE  1023
#define DEBUG      0


typedef enum
{
    RECORD_DATA = 0x00,
    RECORD_EOF  = 0x01,
    RECORD_EX_SEG_ADDR = 0x02,
    RECORD_ST_SEG_ADDR = 0x03,
    RECORD_EX_LNR_ADDR = 0x04,
    RECORD_ST_LNR_ADDR = 0x05
} tRecordType;


/*
* :llaaaatt[dd...]cc
* ||||||||||||||||||
* ||||||||||||||||``- CHECKSUM (two's complement)
* |||||||||```````- DATA (2 * LENGTH of HEX digits)
* |||||||``- TYPE 00: data record
* |||||||         01: end of file record
* |||||||         02: expended segment address record
* |||||||         03: start segment address record
* |||||||         04: expended linear address record
* |||||||         05: start linear address record
* |||````- ADDRESS
* |``- LENGTH
* `-START (colon)
*/
static uint8 _binBuff[65536];


#if DEBUG
static void _dump(void *pAddr, int size)
{
    static int dumpCnt = 3;
    uint8 *p = (uint8 *)pAddr;
    int i;

    if ( dumpCnt )
    {
        if ( pAddr )
        {
            for (i=0; i<size; i++)
            {
                if ((i != 0) && ((i % 16) == 0)) printf("\n");
                printf(" %02X", p[i]);
            }
            printf("\n");
            printf(" (%d bytes)\n", size);
        }
        printf("\n");
        dumpCnt--;
    }
}
#endif

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

int _parseLine(
    char  *pString,
    int    stringLen,
    uint8 *pData,
    int    dataLen,
    bool   checkFlag
)
{
    uint8  nibbleH;
    uint8  nibbleL;
    uint8  byte;
    uint8  csum;
    int i;
    int j;

    if ((stringLen % 2) != 0)
    {
        printf("ERR: wrong input string length %d\n", stringLen);
        return 0;
    }

    if (stringLen > (dataLen << 1))
    {
        printf("ERR: un-enough output buffer length %d\n", dataLen);
        stringLen = (dataLen << 1);
        return 0;
    }

    /*
    * Example:
    *   Input   "11223344"
    *   Output  0x11 0x22 0x33 0x44
    */
    csum = 0;
    for (i=0, j=0; i<stringLen; i+=2)
    {
        nibbleH = _hex2dec( pString[i  ] );
        nibbleL = _hex2dec( pString[i+1] );

        byte  = ((nibbleH << 4) | (nibbleL));
        csum += byte;

        pData[j++] = byte;
    }

    if (( checkFlag ) && (csum != 0))
    {
        printf("ERR: wrong checksum %02Xh\n", csum);
        return 0;
    }

    return j;
}

static int _readLine(FILE *pFile, char *pBuf, int bufSize)
{
    int len = 0;

    pBuf[0] = 0x0;

    if ( feof(pFile) )
    {
        return 0;
    }

    /* char *fgets(                                   */
    /*     char *s,      // character array to store  */
    /*     int   n,      // length to read            */
    /*     FILE *stream  // FILE pointer              */
    /* );                                             */
    fgets(pBuf, bufSize, pFile);

    /* remove the CR/LF character */
    if ((strlen(pBuf) > 0) && (pBuf[strlen(pBuf)-1] == 0x0a))
    {
        pBuf[strlen(pBuf)-1] = 0x0;
    }
    if ((strlen(pBuf) > 0) && (pBuf[strlen(pBuf)-1] == 0x0d))
    {
        pBuf[strlen(pBuf)-1] = 0x0;
    }

    len = strlen( pBuf );

    return len;
}

static long _getFileSize(char *pFileName)
{
    struct stat stat_buf;

    if ( !stat(pFileName, &stat_buf) )
    {
        return stat_buf.st_size;
    }

    return 0;
}

uint8 _checksum(uint8 *pData, int len)
{
    uint8 csum = 0;
    int   i;

    for (i=0; i<len; i++)
    {
        csum += pData[i];
    }

    csum = ((~csum) + 1);

    return csum;
}

char *_extLinearAddr(uint16 addr)
{
    static char text[32];
    uint8 buf[16];

    buf[0] = 0x02;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x04;
    UINT16_TO_BYTE_ARRAY(addr, (buf + 4));
    buf[6] = _checksum(buf, 6);

    sprintf(
        text,
        ":%02X%02X%02X%02X%02X%02X%02X\r\n",
        buf[0],
        buf[1],
        buf[2],
        buf[3],
        buf[4],
        buf[5],
        buf[6]
    );

    return text;
}


/* .mcs -> .bin */
void mcs2bin(char *pFileIn, char *pFileOut)
{
    FILE   *pInput  = NULL;
    FILE   *pOutput = NULL;
    char    text[TEXT_SIZE+1];
    char   *pText;

    uint8  *pBinBuff = &(_binBuff[0]);
    uint8   dummyByte = EMPTY_BYTE;
    uint32  lineCount = 0;
    uint32  byteCount = 0;
    uint32  binLen  = 0;
    uint32  addrInit = 0;
    uint32  addrNext = 0;
    uint16  val16;

    uint8   buf[300];
    int     len;
    int     i;


    memset(_binBuff, EMPTY_BYTE, sizeof(_binBuff));

    if ((pInput=fopen(pFileIn, "r")) == NULL)
    {
        printf("ERR: cannot open file '%s'\n", pFileIn);
        printf("\n");
        goto _EXIT;
    }

    if ((pOutput=fopen(pFileOut, "w")) == NULL)
    {
        printf("ERR: cannot open file '%s'\n", pFileOut);
        printf("\n");
        goto _EXIT;
    }

    while ( _readLine(pInput, text, TEXT_SIZE) )
    {
        //printf("%s\n", text);

        lineCount++;

        /* Start code */
        if (':' == text[0])
        {
            pText = (text + 1);
            len = _parseLine(
                      pText,
                      strlen(pText),
                      buf,
                      300,
                      TRUE
                  );

            #if DEBUG
            _dump(buf, len);
            #endif

            /* Check the record length */
            if ((len < 5) || ((len - 5) < buf[0]))
            {
                printf("ERR: invalid record at line(%d):\n", lineCount);
                printf("'%s'\n", text);
                printf("\n");
                goto _EXIT;
            }

            /* Record type */
            switch ( buf[3] )
            {
                case RECORD_DATA:
                    /* :xxxxxx00........ */
                    BYTE_ARRAY_TO_UINT16((buf + 1), val16);

                    memcpy((pBinBuff + val16), (buf + 4), buf[0]);
                    binLen = (val16 + buf[0]);

                    addrNext = (addrInit + binLen);
                    byteCount += buf[0];

                    #if DEBUG
                    _dump((buf + 4), buf[0]);
                    #endif
                    break;
                case RECORD_EX_SEG_ADDR:
                    /* :02000002.... */
                    break;
                case RECORD_ST_SEG_ADDR:
                    /* :04000003........ */
                    break;
                case RECORD_EX_LNR_ADDR:
                    /* :02000004.... */
                    BYTE_ARRAY_TO_UINT16((buf + 4), val16);
                    addrInit = (val16 << 16);

                    if (binLen > 0)
                    {
                        fwrite(_binBuff, binLen, 1, pOutput);
                    }

                    if (addrNext < addrInit)
                    {
                        #if DEBUG
                        printf("add dummy byte %08X - %08X\n", addrNext, addrInit);
                        #endif
                        for (i=0; i<(addrInit - addrNext); i++)
                        {
                            fwrite(&dummyByte, 1, 1, pOutput);
                        }
                    }

                    memset(_binBuff, EMPTY_BYTE, sizeof(_binBuff));
                    binLen = 0;
                    break;
                case RECORD_ST_LNR_ADDR:
                    /* :04000005........ */
                    break;
                case RECORD_EOF:
                    /* :00000001 */
                    if (binLen > 0)
                    {
                        fwrite(_binBuff, binLen, 1, pOutput);
                    }
                    printf(".mcs -> .bin: input size is %d\n", byteCount);
                    goto _EXIT;
                    break;
                default:
                    printf("ERR: unknown record type 0x%02X\n", buf[3]);
                    break;
            }
        }
    }

_EXIT:
    if ( pInput  ) fclose( pInput );
    if ( pOutput ) fclose( pOutput );
}

/* .bin -> .mcs */
void bin2mcs(char *pFileIn, char *pFileOut)
{
    FILE   *pInput  = NULL;
    FILE   *pOutput = NULL;
    char    text[TEXT_SIZE+1];
    char   *pText;
    int     fileSize;

    uint8   buf[32];
    uint8   csum;
    uint16  addrExt;
    int     remainSize;
    int     blockSize;
    int     totalSize;
    int     next;
    int     i;
    int     j;
    int     k;


    if ((pInput=fopen(pFileIn, "r")) == NULL)
    {
        printf("ERR: cannot open file '%s'\n", pFileIn);
        printf("\n");
        goto _EXIT;
    }

    if ((pOutput=fopen(pFileOut, "w")) == NULL)
    {
        printf("ERR: cannot open file '%s'\n", pFileOut);
        printf("\n");
        goto _EXIT;
    }

    fileSize = _getFileSize( pFileIn );
    if (fileSize <= 0)
    {
        printf("ERR: illegal input file size %d\n", fileSize);
        printf("\n");
        goto _EXIT;
    }

    addrExt    = 0;
    totalSize  = 0;
    remainSize = fileSize;

    fprintf(pOutput, "%s", _extLinearAddr(addrExt));
    addrExt++;

    for (i=0, j=0; i<fileSize; i+=16, j+=16)
    {
        if (j > 0xFFFF)
        {
            fprintf(pOutput, "%s", _extLinearAddr(addrExt));
            addrExt++;
            j = 0;
        }

        if (remainSize < 16)
        {
            blockSize = remainSize;
        }
        else
        {
            blockSize   = 16;
            remainSize -= 16;
        }

        buf[0] = blockSize;
        buf[1] = ((j >> 8) & 0xFF);
        buf[2] = ((j     ) & 0xFF);
        buf[3] = 0x00;

        fread((buf + 4), blockSize, 1, pInput);
        totalSize += blockSize;

        csum = _checksum(buf, (blockSize + 4));

        text[0] = ':';
        pText = (text + 1);
        for (k=0; k<(blockSize + 4); k++)
        {
            next = sprintf(pText, "%02X", buf[k]);
            pText += next;
        }
        sprintf(pText, "%02X\r\n", csum);

        fprintf(pOutput, "%s", text);
    }

    fprintf(pOutput, ":00000001FF\r\n");

    printf(".bin -> .mcs: input size is %d\n", totalSize);

_EXIT:
    if ( pInput  ) fclose( pInput );
    if ( pOutput ) fclose( pOutput );
}

/* calculate checksum */
void hex2checksum(char *pHex)
{
    uint8  csum;
    uint8  buf[300];
    int    len;

    if (strlen( pHex ) > 0)
    {
        len = _parseLine(
                  pHex,
                  strlen(pHex),
                  buf,
                  300,
                  FALSE
              );

        #if DEBUG
        _dump(buf, len);
        #endif

        csum = _checksum(buf, len);
        printf("checksum: %02X\n", csum);
    }
}


void help(void)
{
    printf("Usage: %s <MODE> <FILE_IN> <FILE_OUT>\n", APP_NAME);
    printf("\n");
    printf("%s -e file_in.mcs file_out.bin\n", APP_NAME);
    printf("%s -d file_in.bin file_out.mcs\n", APP_NAME);
    printf("%s -c HEX-string\n", APP_NAME);
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (3 == argc)
    {
        if (0 == strcmp(argv[1], "-c"))
        {
            hex2checksum( argv[2] );
        }
        else
        {
            printf("ERR: unknown mode '%s'\n", argv[1]);
            help();
        }
    }
    else if (4 == argc)
    {
        if (0 == strcmp(argv[1], "-e"))
        {
            mcs2bin(argv[2], argv[3]);
        }
        else if (0 == strcmp(argv[1], "-d"))
        {
            bin2mcs(argv[2], argv[3]);
        }
        else
        {
            printf("ERR: unknown mode '%s'\n", argv[1]);
            help();
        }
    }
    else
    {
        help();
    }

    return 0;
}

