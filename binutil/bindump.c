#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFile = NULL;
    long  size;

    char *pInput = NULL;
    long  offset = 0;
    long  length = 0;
    int   chFlag = 0;
    unsigned char  byte;
    long  i;


    if (argc < 2)
    {
        printf("Usage: bindump [options] INPUT_BIN\n");
        printf("\n");
        printf("  -o OFFSET   Offset from the begin of file\n");
        printf("  -l LENGTH   Length to dump file\n");
        printf("  -c          Dump in character type\n");
        printf("\n");
        return 0;
    }

    i = 1;
    while (i < argc)
    {
        if (i == (argc - 1))
        {
            pInput = argv[i];
            break;
        }
        else
        {
            if (0 == strcmp("-c", argv[i]))
            {
                chFlag = 1;
                i++;
            }
            else if (0 == strcmp("-o", argv[i]))
            {
                i++;
                if (i < argc)
                {
                    offset = atoi( argv[i] );
                    i++;
                }
            }
            else if (0 == strcmp("-l", argv[i]))
            {
                i++;
                if (i < argc)
                {
                    length = atoi( argv[i] );
                    i++;
                }
            }
            else
            {
                i++;
            }
        }
    }


    if (NULL == pInput)
    {
        printf("ERR: no input file\n");
        return 0;
    }

    pFile = fopen(pInput, "rb");
    if (NULL == pFile)
    {
        printf("ERR: cannot open file %s\n", pInput);
        return 0;
    }

    size = filesize( pInput );

    if (offset > 0)
    {
        fseek(pFile, offset, SEEK_SET);
    }
    for (i=0; i<size; i++)
    {
        fread(&byte, 1, 1, pFile);

        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        if (( chFlag ) &&
            ((byte >= 0x20) && (byte <= 0x7E)))
        {
            printf(" %2c", byte);
        }
        else
        {
            printf(" %02X", byte);
        }

        if ((length > 0) && (i >= length)) break;
    }
    printf("\n");
    printf(" (%ld bytes)\n", i);
    printf("\n");

    fclose( pFile );

    return 0;
}

