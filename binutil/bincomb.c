#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"


#define MERGE_FILE_NUM  (16)
#define MERGE_ARG_NUM   ((MERGE_FILE_NUM * 2) + 2)

char *g_fileName[MERGE_FILE_NUM];
int   g_fileNum;
unsigned int   g_combSize[MERGE_FILE_NUM];


/**
*  Merge multiple file to a single file.
*  @param [in]  pFileOut  Output file name.
*/
void merge(char *pFileOut)
{
    FILE *pInput  = NULL;
    FILE *pOutput = NULL;
    unsigned int  finalSize;
    unsigned int  singleSize;

    unsigned char byte;
    unsigned int  j;
    int   i;

    if ((pOutput=fopen(pFileOut, "wb")) == NULL)
    {
        printf("Exit: cannot open file '%s'\n", pFileOut);
        printf("\n");
        goto _EXIT;
    }

    finalSize = 0;

    for (i=0; i<g_fileNum; i++)
    {
        singleSize = filesize( g_fileName[i] );
        //if (singleSize > g_combSize[i])
        //{
        //    printf(
        //        "ERR: illegal input file size (%u <--> %u)\n",
        //        singleSize,
        //        g_combSize[i]
        //    );
        //    printf("\n");
        //    break;
        //}

        if ((pInput=fopen(g_fileName[i], "rb")) == NULL)
        {
            printf("ERR: cannot open file %s\n", g_fileName[i]);
            printf("\n");
            break;
        }

        printf(
            "#%d combine file size:  %u / %u\n",
            (i + 1),
            g_combSize[i],
            singleSize
        );

        for (j=0; j<g_combSize[i]; j++)
        {
            if (j < singleSize)
            {
                fread(&byte, 1, 1, pInput);
            }
            else
            {
                byte = 0x00;
            }

            fwrite(&byte, 1, 1, pOutput);
        }

        fclose( pInput );
        pInput = NULL;

        finalSize += g_combSize[i];
    }

    printf("\n");
    printf("Final merged file size = %u\n", finalSize);
    printf("\n");

_EXIT:
    if ( pInput  ) fclose( pInput );
    if ( pOutput ) fclose( pOutput );
}


void help(void)
{
    // argv[]      0       1      2      3      4          5
    printf("Usage: bincomb FILE_1 SIZE_1 FILE_2 SIZE_2 ... FILE_OUT\n");
    printf("\n");
    printf("EX   : bincomb foo.bin 40 bar.bin 128 test.bin\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int i, j;

    if ((argc < 3) || (argc > MERGE_ARG_NUM) || ((argc % 2) != 0))
    {
        help();
        return 0;
    }

    g_fileNum = ((argc / 2) - 1);

    for (i=0, j=1; i<g_fileNum; i++, j+=2)
    {
        g_fileName[i] = argv[j];
        g_combSize[i] = atoi( argv[j+1] );
    }

    merge( argv[j] );

    return 0;
}

