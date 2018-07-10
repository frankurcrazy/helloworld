#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>


int rb2bw(int rb);

double dl_rasterValue(double freq);

void dl_3MHz(int rb);
void dl_5MHz(int rb);
void dl_10MHz(int rb, int anchor);
void dl_15MHz(int rb, int anchor);
void dl_20MHz(int rb, int anchor);

void ul_3MHz(int rb);
void ul_5MHz(int rb);
void ul_10MHz(int rb);
void ul_15MHz(int rb);
void ul_20MHz(int rb);


#endif
