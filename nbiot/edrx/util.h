#ifndef _UTIL_H_
#define _UTIL_H_


int get_bits(unsigned long long val64);

void print_bits(unsigned long long val64, char *pStr);

int cmp_val64(unsigned long long A, unsigned long long B);

unsigned long long bin_div64(
    unsigned long long  A,
    unsigned long long  B,
    unsigned long long *R
);

unsigned long long calc_fcs(
    unsigned long long Y1,
    unsigned long long Y2
);


#endif
