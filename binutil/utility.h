#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdio.h>


long filesize(char *pName);
void dump(const void *pAddr, unsigned int size);
unsigned char *hexstr2byte(char *pStr, unsigned int *pLen);


#endif
