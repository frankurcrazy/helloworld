#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


void do_ntohl(FILE *pFileIn, long sizeIn, FILE *pFileOut, long sizeOut)
{
    unsigned int   dword;
    unsigned char  byte;
    long  i;
    int   seq;

    seq = 0;
    for (i=0; i<sizeIn; i++)
    {
        if ( feof( pFileIn ) ) break;

        fread(&byte, 1, 1, pFileIn);

        switch ( seq )
        {
            case 0:
                dword  = (byte << 24);
                break;
            case 1:
                dword |= (byte << 16);
                break;
            case 2:
                dword |= (byte <<  8);
                break;
            case 3:
            default:
                dword |= (byte      );
                fwrite(&dword, 4, 1, pFileOut);
                break;
        }

        seq = ((seq + 1) & 0x3);
    }

    if ((i & 0x3) > 0)
    {
        fwrite(&dword, 4, 1, pFileOut);
    }
}

void do_ntohs(FILE *pFileIn, long sizeIn, FILE *pFileOut, long sizeOut)
{
    unsigned short word;
    unsigned char  byte;
    long  i;
    int   seq;

    seq = 0;
    for (i=0; i<sizeIn; i++)
    {
        if ( feof( pFileIn ) ) break;

        fread(&byte, 1, 1, pFileIn);

        switch ( seq )
        {
            case 0:
                word  = (byte << 8);
                break;
            case 1:
            default:
                word |= (byte     );
                fwrite(&word, 2, 1, pFileOut);
                break;
        }

        seq = ((seq + 1) & 0x1);
    }

    if ((i & 0x1) > 0)
    {
        fwrite(&word, 2, 1, pFileOut);
    }
}

void do_htonl(FILE *pFileIn, long sizeIn, FILE *pFileOut, long sizeOut)
{
    unsigned char  byteH[4];
    unsigned char  byteN[4];
    unsigned int  *dword = (unsigned int *)byteH;
    long  i;
    int   seq;

    seq = 0;
    for (i=0; i<sizeIn; i++)
    {
        if ( feof( pFileIn ) ) break;

        if (0 == seq)
        {
            (*dword) = 0;
        }

        fread(&(byteH[seq]), 1, 1, pFileIn);

        if (3 == seq)
        {
            byteN[0] = (((*dword) >> 24) & 0xFF);
            byteN[1] = (((*dword) >> 16) & 0xFF);
            byteN[2] = (((*dword) >>  8) & 0xFF);
            byteN[3] = (((*dword)      ) & 0xFF);

            fwrite(byteN, 4, 1, pFileOut);
        }

        seq = ((seq + 1) & 0x3);
    }

    if ((i & 0x3) > 0)
    {
        byteN[0] = (((*dword) >> 24) & 0xFF);
        byteN[1] = (((*dword) >> 16) & 0xFF);
        byteN[2] = (((*dword) >>  8) & 0xFF);
        byteN[3] = (((*dword)      ) & 0xFF);

        fwrite(byteN, 4, 1, pFileOut);
    }
}

void do_htons(FILE *pFileIn, long sizeIn, FILE *pFileOut, long sizeOut)
{
    unsigned char   byteH[2];
    unsigned char   byteN[2];
    unsigned short *word = (unsigned short *)byteH;
    long  i;
    int   seq;

    seq = 0;
    for (i=0; i<sizeIn; i++)
    {
        if ( feof( pFileIn ) ) break;

        if (0 == seq)
        {
            (*word) = 0;
        }

        fread(&(byteH[seq]), 1, 1, pFileIn);

        if (1 == seq)
        {
            byteN[0] = (((*word) >> 8) & 0xFF);
            byteN[1] = (((*word)     ) & 0xFF);

            fwrite(byteN, 2, 1, pFileOut);
        }

        seq = ((seq + 1) & 0x1);
    }

    if ((i & 0x1) > 0)
    {
        byteN[0] = (((*word) >> 8) & 0xFF);
        byteN[1] = (((*word)     ) & 0xFF);

        fwrite(byteN, 2, 1, pFileOut);
    }
}

void help(void)
{
    printf("Usage: binendian -ntohl FILE_IN FILE_OUT\n");
    printf("     : binendian -ntohs FILE_IN FILE_OUT\n");
    printf("     : binendian -htonl FILE_IN FILE_OUT\n");
    printf("     : binendian -htons FILE_IN FILE_OUT\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    char  order[16] = {0};
    FILE *pFileIn = NULL;
    FILE *pFileOut = NULL;
    long  sizeIn = 0;
    long  sizeOut = 0;


    if (argc != 4)
    {
        help();
        return 0;
    }

    if ((pFileIn=fopen(argv[2], "r")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n\n",
            argv[2]
        );
        return -1;
    }

    if ((pFileOut=fopen(argv[3], "w")) == NULL)
    {
        fprintf(
            stdout,
            "ERR: cannot open %s\n\n",
            argv[3]
        );
        fclose( pFileIn );
        return -1;
    }

    sizeIn = filesize( argv[2] );
    if (sizeIn <= 0)
    {
        fprintf(
            stdout,
            "ERR: wrong file size %ld\n\n",
            sizeIn
        );
        fclose( pFileIn );
        fclose( pFileOut );
        return -1;
    }

    if (0 == strcmp(argv[1], "-ntohl"))
    {
        sprintf(order, "host");

        sizeOut = (sizeIn & (~0x3));
        if ((sizeIn & 0x3) > 0)
        {
            sizeOut += 4;
        }

        do_ntohl(pFileIn, sizeIn, pFileOut, sizeOut);
    }
    else if (0 == strcmp(argv[1], "-ntohs"))
    {
        sprintf(order, "host");

        sizeOut = (sizeIn & (~0x1));
        if ((sizeIn & 0x1) > 0)
        {
            sizeOut += 2;
        }

        do_ntohs(pFileIn, sizeIn, pFileOut, sizeOut);
    }
    else if (0 == strcmp(argv[1], "-htonl"))
    {
        sprintf(order, "network");

        sizeOut = (sizeIn & (~0x3));
        if ((sizeIn & 0x3) > 0)
        {
            sizeOut += 4;
        }

        do_htonl(pFileIn, sizeIn, pFileOut, sizeOut);
    }
    else if (0 == strcmp(argv[1], "-htons"))
    {
        sprintf(order, "network");

        sizeOut = (sizeIn & (~0x1));
        if ((sizeIn & 0x1) > 0)
        {
            sizeOut += 2;
        }

        do_htons(pFileIn, sizeIn, pFileOut, sizeOut);
    }
    else
    {
        help();
        fclose( pFileIn );
        fclose( pFileOut );
        return 0;
    }

    fclose( pFileIn );
    fclose( pFileOut );

    printf("Convert to %s order ... %ld bytes\n\n", order, sizeOut);
    return 0;
}

