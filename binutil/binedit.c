#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFile;
    unsigned int   size1;
    unsigned int   size2;
    unsigned int   offset;
    unsigned char *pValue;
    unsigned int   valueLen;
    unsigned int   i;


    if (argc < 4)
    {
        printf("Usage: binedit INPUT_BIN OFFSET HEX_STRING\n");
        printf("\n");
        return 0;
    }

    size1 = filesize( argv[1] );
    offset = 0;

    if ((pFile=fopen(argv[1], "r+")) == NULL)
    {
        printf("ERR: cannot open %s\n", argv[1]);
        return -1;
    }

    if ((strlen(argv[2]) > 2) &&
        (argv[2][0] == '0') && (argv[2][1] == 'x'))
    {
        sscanf(argv[2], "0x%x", &offset);
    }
    else
    {
        offset = atoi( argv[2] );
    }

    pValue = hexstr2byte(argv[3], &valueLen);
    if (( pValue ) && (valueLen > 0))
    {
        //dump(pValue, valueLen);

        if (offset > 0)
        {
            fseek(pFile, offset, SEEK_SET);
        }

        for (i=0; i<valueLen; i++)
        {
            fwrite(&(pValue[i]), 1, 1, pFile);
        }

        free( pValue );
    }

    fclose( pFile );

    size2 = filesize( argv[1] );
    if (size1 != size2)
    {
        printf("File size changed: %u -> %u\n\n", size1, size2);
    }

    return 0;
}

