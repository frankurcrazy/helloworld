#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFileIn;
    unsigned char  byte;

    unsigned int   totalLen;
    unsigned int   offset;

    unsigned char *pPattern;
    unsigned int   patternLen;
    int  next;
    int  count;
    int  i;


    if (argc != 3)
    {
        printf("Usage: binfind FILE PATTERN\n");
        printf("\n");
        return 0;
    }

    totalLen = filesize( argv[1] );
    offset   = 0;

    if ((pFileIn=fopen(argv[1], "rb")) == NULL)
    {
        printf("ERR: cannot open %s\n", argv[1]);
        return -1;
    }

    pPattern = hexstr2byte(argv[2], &patternLen);
    if (( pPattern ) && (patternLen > 0))
    {
        //dump(pPattern, patternLen);

        next  = 0;
        count = 1;
        for (i=0; i<totalLen; i++)
        {
            fread(&byte, 1, 1, pFileIn);

            if (byte == pPattern[next])
            {
                if (next == 0) offset = i;

                next++;
                if (next == patternLen)
                {
                    printf("#%d found at 0x%X\n", count, offset);
                    count++;
                }
            }
            else
            {
                next = 0;
            }
        }

        free( pPattern );
    }

    fclose( pFileIn );

    return 0;
}

