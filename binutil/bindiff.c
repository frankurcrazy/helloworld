#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    FILE *pFileA;
    FILE *pFileB;
    unsigned char  byteA;
    unsigned char  byteB;
    unsigned int   lenA;
    unsigned int   lenB;
    unsigned int   cmpSize;

    int  count;
    int  i;


    if (argc != 3)
    {
        printf("Usage: bindiff FILE_1 FILE_2\n");
        printf("\n");
        return 0;
    }

    lenA = filesize( argv[1] );
    lenB = filesize( argv[2] );
    cmpSize = ((lenA > lenB) ? lenB : lenA);

    if ((pFileA=fopen(argv[1], "rb")) == NULL)
    {
        printf("ERR: cannot open %s\n", argv[1]);
        return -1;
    }

    if ((pFileB=fopen(argv[2], "rb")) == NULL)
    {
        printf("ERR: cannot open %s\n", argv[2]);
        fclose( pFileA );
        return -1;
    }

    count = 0;
    for (i=0; i<cmpSize; i++)
    {
        fread(&byteA, 1, 1, pFileA);
        fread(&byteB, 1, 1, pFileB);

        if (byteA != byteB)
        {
            count++;
            if (count <= 10)
            {
                printf(
                    "#%d (%02X <--> %02X) at 0x%X\n",
                    count,
                    byteA,
                    byteB,
                    i
                );
            }
        }
    }

    printf("Total has %d difference(s)\n\n", count);

    fclose( pFileA );
    fclose( pFileB );

    return 0;
}

