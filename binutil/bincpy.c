#include <stdio.h>
#include <stdlib.h>
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


    if (argc != 5)
    {
        printf("Usage: bincpy FILE_IN FILE_OUT START LENGTH\n");
        printf("\n");
        return 0;
    }


    if ((pFileIn=fopen(argv[1], "rb")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n",
            argv[1]
        );
        return -1;
    }

    if ((pFileOut=fopen(argv[2], "wb")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n",
            argv[2]
        );
        fclose( pFileIn );
        return -1;
    }

    start  = atoi( argv[3] );
    length = atoi( argv[4] );

    if (start > 0)
    {
        fseek(pFileIn, start, SEEK_SET);
    }
    for (i=0; i<length; i++)
    {
        fread(&buf, 1, 1, pFileIn);
        fwrite(&buf, 1, 1, pFileOut);
    }

    fclose( pFileIn );
    fclose( pFileOut );

    printf("Binary copy ... %u bytes\n", length);
    return 0;
}

