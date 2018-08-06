#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int framePeriod = 8;
    int frameOffset = 2;
    char *oneFrame = "110000";

    int frameNum = 0;
    int loops = 256;
    int i;


    if ((argc == 2) && (0 == strcmp("-h", argv[1])))
    {
        printf("Usage: subframe [frame period]\n");
        printf("     : subframe [frame period] [frame offset]\n");
        printf("     : subframe [frame period] [frame offset] [subframe allocation]\n");
        printf("\n");
        return 0;
    }

    if (argc > 3)
    {
        framePeriod = atoi( argv[1] );
        frameOffset = atoi( argv[2] );
        oneFrame = argv[3];
    }
    else if (argc > 2)
    {
        framePeriod = atoi( argv[1] );
        frameOffset = atoi( argv[2] );
    }
    else if (argc > 1)
    {
        framePeriod = atoi( argv[1] );
    }

    printf("Frame period = %d\n", framePeriod);
    printf("Frame offset = %d\n", frameOffset);
    printf("Subframe allocation = \"%s\"\n", oneFrame);
    printf("\n");

    printf("Frame :: Subframe\n");
    printf("      :: 0 1 2 3 4 5 6 7 8 9\n");
    printf("------------------------------\n");
    for (i=0; i<loops; i++)
    {
        if ((frameNum % framePeriod) == frameOffset)
        {
            printf(
                "%4d  :: x %d %d %d x x %d %d %d x\n",
                frameNum,
                (oneFrame[0] == '1'),
                (oneFrame[1] == '1'),
                (oneFrame[2] == '1'),
                (oneFrame[3] == '1'),
                (oneFrame[4] == '1'),
                (oneFrame[5] == '1')
            );
        }

        frameNum++;
        if (frameNum > 1023) frameNum = 0;
    }
    printf("\n");

    return 0;
}

