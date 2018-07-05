#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


long filesize(char *pName)
{
    struct stat  stat_buf;

    if ( !stat(pName, &stat_buf) )
    {
        return stat_buf.st_size;
    }

    return 0;
}

void dump(const void *pAddr, unsigned int size)
{
    unsigned char *pByte = (unsigned char *)pAddr;
    unsigned int  i;

    if (pAddr == NULL)
    {
        printf("NULL pointer\n\n");
        return;
    }

    printf("size = %d\n", size);
    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        printf(" %02X", pByte[i]);
    }
    printf("\n\n");
}

unsigned char *hexstr2byte(char *pStr, unsigned int *pLen)
{
    unsigned char *pBuf = NULL;
    unsigned char  val1 = 0;
    unsigned char  val2 = 0;
    unsigned int   bufSize = 0;
    unsigned int   i;
    unsigned int   j;
    int   ignore;
    char  ch;

    *(pLen) = 0;

    pBuf = malloc(((strlen( pStr ) + 1) / 2) + 1);
    if ( pBuf )
    {
        i = j = 0;
        while ( (ch = pStr[i]) )
        {
            ignore = 0;

            switch ( ch )
            {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    val1 = (ch - '0');
                    break;
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    val1 = (((ch - 'a')) + 10);
                    break;
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    val1 = (((ch - 'A')) + 10);
                    break;
                case 0x20:
                    ignore = 1;
                    break;
                default:
                    printf("incorrect HEX string '%c'\n", ch);
                    free( pBuf );
                    return NULL;
            }

            if ( !ignore )
            {
                if (0 == (j % 2))
                {
                    val2 = (val1 << 4);
                }
                else
                {
                    val2 += val1;

                    pBuf[j / 2] = val2;
                    bufSize++;
                }

                j++;
            }

            i++;
        }

        if ((j % 2) != 0)
        {
            printf("Hex string is not byte-aligned\n");
            free( pBuf );
            return NULL;
        }
    }

    *(pLen) = bufSize;
    return pBuf;
}

