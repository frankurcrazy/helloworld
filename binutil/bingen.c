#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFileIn  = NULL;
    FILE *pFileOut = NULL;
    long  sizeIn;
    long  sizeOut;
    char  text[1024];

    void *pData;
    unsigned int  bytes;
    long  i;
    long  j;


    if (argc != 3)
    {
        printf("Usage: bingen INPUT_TXT OUTPUT_BIN\n");
        printf("\n");
        return 0;
    }

    pFileIn = fopen(argv[1], "r");
    if (NULL == pFileIn)
    {
        printf("ERR: cannot open file %s\n", argv[1]);
        return 0;
    }

    pFileOut = fopen(argv[2], "w");
    if (NULL == pFileOut)
    {
        printf("ERR: cannot open file %s\n", argv[2]);
        fclose( pFileIn );
        return 0;
    }

    sizeIn  = filesize( argv[1] );
    sizeOut = 0;

    for (i=0, j=0; i<sizeIn; i++)
    {
        fread(&(text[j]), 1, 1, pFileIn);
        if (('\r' == text[j]) || ('\n' == text[j]))
        {
            text[j] = '\0';
            j = 0;

            if ( text[0] )
            {
                pData = hexstr2byte(text, &bytes);
                if ( pData )
                {
                    if (bytes > 0)
                    {
                        fwrite(pData, bytes, 1, pFileOut);
                        sizeOut += bytes;
                    }

                    free( pData );
                }
            }
        }
        else
        {
            j++;
        }
    }

    if ((j > 0) && ( text[j-1] ))
    {
        pData = hexstr2byte(text, &bytes);
        if ( pData )
        {
            if (bytes > 0)
            {
                fwrite(pData, bytes, 1, pFileOut);
                sizeOut += bytes;
            }

            free( pData );
        }
    }

    fclose( pFileIn  );
    fclose( pFileOut );

    printf(
        "Generating .txt -> .bin (size %ld)\n\n",
        sizeOut
    );

    return 0;
}

