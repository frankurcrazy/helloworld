#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


int main(int argc, char *argv[])
{
    unsigned char  byte;
    unsigned char *pVal1;
    unsigned char *pVal2;
    unsigned int   valLen1;
    unsigned int   valLen2;
    unsigned int   offset1;
    unsigned int   offset2;
    unsigned int   size;
    unsigned int   i;


    if (argc < 3)
    {
        printf("Usage: binxor HEX_STRING_1 HEX_STRING_2\n");
        printf("\n");
        return 0;
    }

    pVal1 = hexstr2byte(argv[1], &valLen1);
    if (( pVal1 ) && (valLen1 > 0))
    {
        pVal2 = hexstr2byte(argv[2], &valLen2);
        if (( pVal2 ) && (valLen2 > 0))
        {
            if (valLen1 > valLen2)
            {
                offset1 = (valLen1 - valLen2);
                offset2 = 0;
                size = valLen2;
                for (i=0; i<offset1; i++)
                {
                    printf("%02X", pVal1[i]);
                }
            }
            else
            {
                offset1 = 0;
                offset2 = (valLen2 - valLen1);
                size = valLen1;
                for (i=0; i<offset2; i++)
                {
                    printf("%02X", pVal2[i]);
                }
            }

            for (i=0; i<size; i++)
            {
                byte = (pVal1[i + offset1] ^ pVal2[i + offset2]);
                printf("%02X", byte);
            }
            printf("\n");

            free( pVal2 );
        }

        free( pVal1 );
    }

    return 0;
}

