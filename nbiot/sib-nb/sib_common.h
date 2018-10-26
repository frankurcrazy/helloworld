#ifndef _SIB_COMMON_H_
#define _SIB_COMMON_H_


/* Integers */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


/* SIBx-NB available mask */
extern uint16  g_sib1Subframe[1024];
extern uint16  g_sib2Subframe[1024];

/* SIBx-NB repetition subframe */
extern uint16  g_sib1Repetition[1024];
extern uint16  g_sib2Repetition[1024];


void sib1Nb_schedule(
    int  cellId,
    int  schInfo,
    int *pRepNum,
    int *pTbSize,
    int *pStartFrm
);
void sib2Nb_schedule(int period, int repetition, int window);


#endif // _SIB_COMMON_H_
