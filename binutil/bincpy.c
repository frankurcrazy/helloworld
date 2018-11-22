#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFileIn;
    FILE *pFileOut;
    unsigned char  buf;
    unsigned int   start;
    unsigned int   length;
    unsigned int   i;
    long  sizeIn;


    if (argc != 5)
    {
        printf("Usage: bincpy FILE_IN FILE_OUT START LENGTH\n");
        printf("\n");
        return 0;
    }


    sizeIn = filesize( argv[1] );

    if ((pFileIn=fopen(argv[1], "r")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n\n",
            argv[1]
        );
        return -1;
    }

    if ((pFileOut=fopen(argv[2], "w")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n\n",
            argv[2]
        );
        fclose( pFileIn );
        return -1;
    }

    if ((strlen(argv[3]) > 2) &&
        (argv[3][0] == '0') && (argv[3][1] == 'x'))
    {
        sscanf(argv[3], "0x%x", &start);
    }
    else
    {
        start = atoi( argv[3] );
    }

    if ((strlen(argv[4]) > 2) &&
        (argv[4][0] == '0') && (argv[4][1] == 'x'))
    {
        sscanf(argv[4], "0x%x", &length);
    }
    else
    {
        length = atoi( argv[4] );
    }

    if (start > 0)
    {
        if ((start >= sizeIn) || (0 != fseek(pFileIn, start, SEEK_SET)))
        {
            fclose( pFileIn );
            fclose( pFileOut );
            fprintf(
                stdout,
                "ERR: wrong start %u\n\n",
                start
            );
            return -1;
        }
    }
    for (i=0; i<length; i++)
    {
        if ( feof( pFileIn ) ) break;
        fread(&buf,  1, 1, pFileIn);
        fwrite(&buf, 1, 1, pFileOut);
    }

    fclose( pFileIn );
    fclose( pFileOut );

    printf("Binary copy ... %u bytes\n", i);
    return 0;
}

