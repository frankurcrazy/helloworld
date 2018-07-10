#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void schedMibNb(void)
{
    int blockIndex;
    int frame;
    int subframe;


    for (frame=0; frame<1024; frame++)
    {
        for (subframe=0; subframe<10; subframe++)
        {
            if (0 == subframe)
            {
                /* start a new MIB-NB at this subframe */
                if (0 == (frame & 0x3f))
                {
                    printf("\nMIB-NB(TB):");
                }

                if (0 == (frame & 0x7)) printf("\n");
                blockIndex = ((frame >> 3) & 0x7);
                printf(" %4d(%2d)", frame, blockIndex);
                if (0x3F == (frame & 0x3F)) printf("\n");
            }
        }
    }
    printf("\n");
}

/*
*  R13 36.211 Section 10.2.4 Narrowband physical broadcast channel
*/
int main(int argc, char *argv[])
{
    schedMibNb();

    return 0;
}

