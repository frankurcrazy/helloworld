#include <stdio.h>


int rb2bw(int rb)
{
    int bw = 0;

    switch ( rb )
    {
        case 6:
            bw = 1400000;
            break;
        case 15:
            bw = 3000000;
            break;
        case 25:
            bw = 5000000;
            break;
        case 50:
            bw = 10000000;
            break;
        case 75:
            bw = 15000000;
            break;
        case 100:
            bw = 20000000;
            break;
        default:
            ;
    }

    return bw;  /* Hz */
}

double dl_rasterValue(double freq)
{
    double start[2];
    double val = 0;

    if (freq > 0)
    {
        start[0] = 0;
        start[1] = 0;
        while (freq > start[0])
        {
            start[1] = start[0];
            start[0] += 100;
        }

        if ((start[0] - freq) > (freq - start[1]))
        {
            val = (freq - start[1]);
        }
        else
        {
            val = (freq - start[0]);
        }
    }
    else
    {
        start[0] = 0;
        start[1] = 0;
        while (start[0] > freq)
        {
            start[1] = start[0];
            start[0] -= 100;
        }

        if ((start[0] - freq) < (freq - start[1]))
        {
            val = (freq - start[1]);
        }
        else
        {
            val = (freq - start[0]);
        }
    }

    return val;
}

void dl_3MHz(int rb)
{
    int center = 7;
    double offset;
    double raster;
    double f1;
    double f2;

    if (rb > center)
    {
        offset = 7.5;
    }
    else if (rb < center)
    {
        offset = -7.5;
    }
    else
    {
        offset = 0;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);
    raster = dl_rasterValue( f1 );

    printf("[1;33m%2d[0m   %+.1f     %+.1f KHz    \t %+.0f\n",
        rb,
        raster,
        f1,
        f2
    );
}

void dl_5MHz(int rb)
{
    int center = 12;
    double offset;
    double raster;
    double f1;
    double f2;

    if (rb > center)
    {
        offset = 7.5;
    }
    else if (rb < center)
    {
        offset = -7.5;
    }
    else
    {
        offset = 0;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);
    raster = dl_rasterValue( f1 );

    printf("[1;33m%2d[0m   %+.1f     %+.1f KHz    \t %+.0f",
        rb,
        raster,
        f1,
        f2
    );
    printf("\n");
}

void dl_10MHz(int rb, int anchor)
{
    int center;
    double offset;
    double raster;
    double f1;
    double f2;

    if (rb >= 25)
    {
        center = 25;
        offset = 97.5;
    }
    else
    {
        center = 24;
        offset = -97.5;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);
    raster = dl_rasterValue( f1 );

    printf("[1;33m%2d[0m   %+.1f     %+.1f KHz    \t %+.0f",
        rb,
        raster,
        f1,
        f2
    );
    if ( anchor )
    {
        printf("\t[1;35m(anchor)[0m");
    }
    printf("\n");
}

void dl_15MHz(int rb, int anchor)
{
    int center = 37;
    double offset;
    double raster;
    double f1;
    double f2;

    if (rb > center)
    {
        offset = 7.5;
    }
    else if (rb < center)
    {
        offset = -7.5;
    }
    else
    {
        offset = 0;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);
    raster = dl_rasterValue( f1 );

    printf("[1;33m%2d[0m   %+.1f     %+.1f KHz    \t %+.0f",
        rb,
        raster,
        f1,
        f2
    );
    if ( anchor )
    {
        printf("\t[1;35m(anchor)[0m");
    }
    printf("\n");
}

void dl_20MHz(int rb, int anchor)
{
    int center;
    double offset;
    double raster;
    double f1;
    double f2;

    if (rb >= 50)
    {
        center = 50;
        offset = 97.5;
    }
    else
    {
        center = 49;
        offset = -97.5;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);
    raster = dl_rasterValue( f1 );

    printf("[1;33m%2d[0m   %+.1f     %+.1f KHz    \t %+.0f",
        rb,
        raster,
        f1,
        f2
    );
    if ( anchor )
    {
        printf("\t[1;35m(anchor)[0m");
    }
    printf("\n");
}

void ul_3MHz(int rb)
{
    int center = 7;
    double offset;
    double f1;
    double f2;

    offset = 0;
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);

    printf("[1;33m%2d[0m   %+.1f KHz    \t%+.0f\n",
        rb,
        f1,
        f2
    );
}

void ul_5MHz(int rb)
{
    int center = 12;
    double offset;
    double f1;
    double f2;

    offset = 0;
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);

    printf("[1;33m%2d[0m   %+.1f KHz    \t%+.0f\n",
        rb,
        f1,
        f2
    );
}

void ul_10MHz(int rb)
{
    int center;
    double offset;
    double f1;
    double f2;

    if (rb >= 25)
    {
        center = 25;
        offset = 90;
    }
    else
    {
        center = 24;
        offset = -90;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);

    printf("[1;33m%2d[0m   %+.1f KHz    \t%+.0f\n",
        rb,
        f1,
        f2
    );
}

void ul_15MHz(int rb)
{
    int center = 37;
    double offset;
    double f1;
    double f2;

    offset = 0;
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);

    printf("[1;33m%2d[0m   %+.1f KHz    \t%+.0f\n",
        rb,
        f1,
        f2
    );
}

void ul_20MHz(int rb)
{
    int center;
    double offset;
    double f1;
    double f2;

    if (rb >= 50)
    {
        center = 50;
        offset = 90;
    }
    else
    {
        center = 49;
        offset = -90;
    }
    f1 = (((rb - center) * 180) + offset);
    f2 = ((f1 * 1000) / 500);

    printf("[1;33m%2d[0m   %+.1f KHz    \t%+.0f\n",
        rb,
        f1,
        f2
    );
}

