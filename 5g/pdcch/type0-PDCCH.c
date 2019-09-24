#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct _tResource
{
    int pattern;
    int rbNum;
    int symbNum;
    int offset;
} tResource;

typedef struct _tPattern
{
    int O;
    int M;
    int ssNum;
    int symb1st;
    int SFNc;
    int nc;
} tPattern;


int N_frame_slot[5] = {
    10, 20, 40, 80, 160
};


tResource *Table_13_1(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 2;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 4;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 2;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 4;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 12;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 16;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 12;
            break;
        case 9:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 16;
            break;
        case 10:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 12;
            break;
        case 11:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 16;
            break;
        case 12:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 38;
            break;
        case 13:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 38;
            break;
        case 14:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 3;
            _resource.offset = 38;
            break;
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_2(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 5;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 6;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 7;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 8;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 5;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 6;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 7;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 8;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 18;
            break;
        case 9:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 20;
            break;
        case 10:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 18;
            break;
        case 11:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 20;
            break;
        case 12:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 18;
            break;
        case 13:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 20;
            break;
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_3(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 2;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 6;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 2;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 6;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 2;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 6;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 28;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 28;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 3;
            _resource.offset = 28;
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_4(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 1;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 2;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 3;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 4;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 1;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 2;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 3;
            break;
        case 9:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 4;
            break;
        case 10:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 12;
            break;
        case 11:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 14;
            break;
        case 12:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 16;
            break;
        case 13:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 12;
            break;
        case 14:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 14;
            break;
        case 15:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 16;
            break;
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_5(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 4;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 4;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 4;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 0;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 56;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 56;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 3;
            _resource.offset = 56;
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_6(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 4;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 3;
            _resource.offset = 4;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 0;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 28;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 28;
            break;
        case 8:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 9:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 28;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_7(int index, int kSSB)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 8;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 8;
            break;
        case 4:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 0;
            break;
        case 5:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 3;
            _resource.offset = 8;
            break;
        case 6:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 28;
            break;
        case 7:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 28;
            break;
        case 8:
            _resource.pattern = 2;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = ((kSSB == 0) ? -41 : -42);
            break;
        case 9:
            _resource.pattern = 2;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 49;
            break;
        case 10:
            _resource.pattern = 2;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = ((kSSB == 0) ? -41 : -42);
            break;
        case 11:
            _resource.pattern = 2;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 97;
            break;
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_8(int index, int kSSB)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 4;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 14;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 14;
            break;
        case 4:
            _resource.pattern = 3;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = ((kSSB == 0) ? -20 : -21);
            break;
        case 5:
            _resource.pattern = 3;
            _resource.rbNum = 24;
            _resource.symbNum = 2;
            _resource.offset = 24;
            break;
        case 6:
            _resource.pattern = 3;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = ((kSSB == 0) ? -20 : -21);
            break;
        case 7:
            _resource.pattern = 3;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 48;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_9(int index)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 1;
            _resource.offset = 16;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 96;
            _resource.symbNum = 2;
            _resource.offset = 16;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}

tResource *Table_13_10(int index, int kSSB)
{
    static tResource _resource;

    switch ( index )
    {
        case 0:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 0;
            break;
        case 1:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 8;
            break;
        case 2:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 0;
            break;
        case 3:
            _resource.pattern = 1;
            _resource.rbNum = 48;
            _resource.symbNum = 2;
            _resource.offset = 8;
            break;
        case 4:
            _resource.pattern = 2;
            _resource.rbNum = 24;
            _resource.symbNum = 1;
            _resource.offset = ((kSSB == 0) ? -41 : -42);
            break;
        case 5:
            _resource.pattern = 2;
            _resource.rbNum = 24;
            _resource.symbNum = 1;
            _resource.offset = 25;
            break;
        case 6:
            _resource.pattern = 2;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = ((kSSB == 0) ? -41 : -42);
            break;
        case 7:
            _resource.pattern = 2;
            _resource.rbNum = 48;
            _resource.symbNum = 1;
            _resource.offset = 49;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_resource;
}


tPattern *Table_13_11(int index, int i, int N_CORESET_symb)
{
    static tPattern _pattern;

    switch ( index )
    {
        case 0:
            _pattern.O = 0;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 1:
            _pattern.O = 0;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 2:
            _pattern.O = 4;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 3:
            _pattern.O = 4;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 4:
            _pattern.O = 10;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 5:
            _pattern.O = 10;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 6:
            _pattern.O = 14;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 7:
            _pattern.O = 14;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 8:
            _pattern.O = 0;
            _pattern.M = 4;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 9:
            _pattern.O = 10;
            _pattern.M = 4;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 10:
            _pattern.O = 0;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 1;
            break;
        case 11:
            _pattern.O = 0;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 2;
            break;
        case 12:
            _pattern.O = 4;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 1;
            break;
        case 13:
            _pattern.O = 4;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 2;
            break;
        case 14:
            _pattern.O = 10;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 1;
            break;
        case 15:
            _pattern.O = 10;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 2;
            break;
        default:
            return NULL;
    }

    return &_pattern;
}

tPattern *Table_13_12(int index, int i, int N_CORESET_symb)
{
    static tPattern _pattern;

    switch ( index )
    {
        case 0:
            _pattern.O = 0;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 1:
            _pattern.O = 0;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : 7);
            break;
        case 2:
            _pattern.O = 5;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 3:
            _pattern.O = 5;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : 7);
            break;
        case 4:
            _pattern.O = 10;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 5:
            _pattern.O = 10;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : 7);
            break;
        case 6:
            _pattern.O = 0;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 7:
            _pattern.O = 5;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 8:
            _pattern.O = 10;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 9:
            _pattern.O = 14;
            _pattern.M = 2;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 10:
            _pattern.O = 14;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : 7);
            break;
        case 11:
            _pattern.O = 14;
            _pattern.M = 1;
            _pattern.ssNum = 2;
            _pattern.symb1st = ((0 == (i % 2)) ? 0 : N_CORESET_symb);
            break;
        case 12:
            _pattern.O = 0;
            _pattern.M = 4;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 13:
            _pattern.O = 10;
            _pattern.M = 4;
            _pattern.ssNum = 1;
            _pattern.symb1st = 0;
            break;
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_pattern;
}

tPattern *Table_13_13(int index, int i, int SFN_SSB_i, int n_SSB_i)
{
    static tPattern _pattern;

    switch ( index )
    {
        case 0:
            _pattern.symb1st = (((i % 4) == 0) ? 0 :
                                ((i % 4) == 1) ? 1 :
                                ((i % 4) == 2) ? 6 : 7);
            _pattern.SFNc = SFN_SSB_i;
            _pattern.nc = n_SSB_i;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_pattern;
}

tPattern *Table_13_14(int index, int i, int SFN_SSB_i, int n_SSB_i)
{
    static tPattern _pattern;

    switch ( index )
    {
        case 0:
            _pattern.symb1st = (((i % 8) == 0) ? 0 :
                                ((i % 8) == 1) ? 1 :
                                ((i % 8) == 2) ? 2 :
                                ((i % 8) == 3) ? 3 :
                                ((i % 8) == 4) ? 12 :
                                ((i % 8) == 5) ? 13 :
                                ((i % 8) == 6) ? 0 : 1);
            _pattern.SFNc = SFN_SSB_i;
            _pattern.nc = ((((i % 8) == 0) || ((i % 8) == 1) || ((i % 8) == 2) ||
                            ((i % 8) == 3) || ((i % 8) == 6) || ((i % 8) == 7))
                           ? n_SSB_i
                           : (n_SSB_i - 1));
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_pattern;
}

tPattern *Table_13_15(int index, int i, int SFN_SSB_i, int n_SSB_i)
{
    static tPattern _pattern;

    switch ( index )
    {
        case 0:
            _pattern.symb1st = (((i % 4) == 0) ? 4 :
                                ((i % 4) == 1) ? 8 :
                                ((i % 4) == 2) ? 2 : 6);
            _pattern.SFNc = SFN_SSB_i;
            _pattern.nc = n_SSB_i;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return NULL;
    }

    return &_pattern;
}


void help(void)
{
    printf("Usage: type0-PDCCH [OPTION]...\n");
    printf("\n");
    printf("  -c coresetId   CORESET with ID #0 (0 ~ 15).\n");
    printf("  -d cssId       Common search space with ID #0 (0 ~ 15).\n");
    printf("  -s ssbSCS      SSB subcarrier spacing (15, 30, 120, 240 KHz).\n");
    printf("  -p pdcchSCS    PDCCH subcarrier spacing (15, 30, 60, 120 KHz).\n");
    printf("  -w miniBW      Minimum channel bandwidth (5, 10, 40 MHz).\n");
    printf("  -i ssbIndex    SSB index (0 ~ 63).\n");
    printf("  -f SFN_SSB_i   SSB system frame number at index i. (0 ~ 1023)\n");
    printf("  -n n_SSB_i     SSB slot number at index i. (0 ~ 159)\n");
    printf("  -k kSSB        K_SSB value.\n");
    printf("  -h             Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 13 UE procedure for monitoring Type0-PDCCH CSS sets */
int main(int argc, char *argv[])
{
    int coresetId = 0;
    int cssId = 0;
    int ssbSCS = 15;
    int pdcchSCS = 15;
    int miniBW = 5;
    int ssbIndex = 0;
    int SFN_SSB_i = 0;
    int n_SSB_i = 0;
    int kSSB = 0;
    int FR = 1;
    int u = 0;

    tResource *pResource = NULL;
    tPattern  *pPattern  = NULL;
    int n0;
    int i;
    int j;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:d:s:p:w:i:f:n:k:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                coresetId = atoi( optarg );
                if ((coresetId < 0) || (coresetId > 15))
                {
                    printf("ERR: wrong CORESET ID %d\n\n", coresetId);
                    return -1;
                }
                break;
            case 'd':
                cssId = atoi( optarg );
                if ((cssId < 0) || (cssId > 15))
                {
                    printf("ERR: wrong CSS ID %d\n\n", cssId);
                    return -1;
                }
                break;
            case 's':
                ssbSCS = atoi( optarg );
                break;
            case 'p':
                pdcchSCS = atoi( optarg );
                break;
            case 'w':
                miniBW = atoi( optarg );
                if ((miniBW != 5) && (miniBW != 10) && (miniBW != 40))
                {
                    printf("ERR: wrong minimum bandwidth %d MHz\n\n", miniBW);
                    return -1;
                }
                break;
            case 'i':
                ssbIndex = atoi( optarg );
                if ((ssbIndex < 0) || (ssbIndex > 63))
                {
                    printf("ERR: wrong SSB index %d\n\n", ssbIndex);
                    return -1;
                }
                break;
            case 'f':
                SFN_SSB_i = atoi( optarg );
                if ((SFN_SSB_i < 0) || (SFN_SSB_i > 1023))
                {
                    printf("ERR: wrong system frame index %d\n\n", SFN_SSB_i);
                    return -1;
                }
                break;
            case 'n':
                n_SSB_i = atoi( optarg );
                if ((n_SSB_i < 0) || (n_SSB_i > 159))
                {
                    printf("ERR: wrong slot index %d\n\n", n_SSB_i);
                    return -1;
                }
                break;
            case 'k':
                kSSB = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((ssbSCS == 15) && (pdcchSCS == 15) && ((miniBW == 5) || (miniBW == 10)))
    {
        pResource = Table_13_1( coresetId );
        FR = 1;
        u = 0;
    }
    else if ((ssbSCS == 15) && (pdcchSCS == 30) && ((miniBW == 5) || (miniBW == 10)))
    {
        pResource = Table_13_2( coresetId );
        FR = 1;
        u = 1;
    }
    else if ((ssbSCS == 30) && (pdcchSCS == 15) && ((miniBW == 5) || (miniBW == 10)))
    {
        pResource = Table_13_3( coresetId );
        FR = 1;
        u = 0;
    }
    else if ((ssbSCS == 30) && (pdcchSCS == 30) && ((miniBW == 5) || (miniBW == 10)))
    {
        pResource = Table_13_4( coresetId );
        FR = 1;
        u = 1;
    }
    else if ((ssbSCS == 30) && (pdcchSCS == 15) && (miniBW == 40))
    {
        pResource = Table_13_5( coresetId );
        FR = 1;
        u = 0;
    }
    else if ((ssbSCS == 30) && (pdcchSCS == 30) && (miniBW == 40))
    {
        pResource = Table_13_6( coresetId );
        FR = 1;
        u = 1;
    }
    else if ((ssbSCS == 120) && (pdcchSCS == 60))
    {
        pResource = Table_13_7(coresetId, kSSB);
        FR = 2;
        u = 2;
    }
    else if ((ssbSCS == 120) && (pdcchSCS == 120))
    {
        pResource = Table_13_8(coresetId, kSSB);
        FR = 2;
        u = 3;
    }
    else if ((ssbSCS == 240) && (pdcchSCS == 60))
    {
        pResource = Table_13_9( coresetId );
        FR = 2;
        u = 2;
    }
    else if ((ssbSCS == 240) && (pdcchSCS == 120))
    {
        pResource = Table_13_10(coresetId, kSSB);
        FR = 2;
        u = 3;
    }
    else
    {
        printf("ERR: wrong SCS {%d, %d} KHz\n\n", ssbSCS, pdcchSCS);
        return -1;
    }

    if ( pResource )
    {
        printf("SS/PBCH block subcarrier spacing %d KHz\n", ssbSCS);
        printf("PDCCH         subcarrier spacing %d KHz\n", pdcchSCS);
        printf("Minimum channel bandwidth %d MHz\n", miniBW);
        printf("\n");

        printf("Multiplexing pattern[ %d ]\n", pResource->pattern);
        printf("  + N_CORESET_RB[ %d ]\n", pResource->rbNum);
        printf("  + N_CORESET_symb[ %d ]\n", pResource->symbNum);
        printf("  + offset[ %d ]\n", pResource->offset);
        printf("\n");

        if (1 == pResource->pattern)
        {
            pPattern = ((FR == 1) ? Table_13_11(cssId, ssbIndex, pResource->symbNum)
                                  : Table_13_12(cssId, ssbIndex, pResource->symbNum));
            if ( pPattern )
            {
                int O = pPattern->O;
                int M = pPattern->M;
                int symb1st = pPattern->symb1st;

                #if 0
                printf("O = %d, M = %d, symb1st = %d\n", O, M, symb1st);
                #endif

                n0 = ((((O << u) >> 1) + ((ssbIndex * M) >> 1)) % N_frame_slot[u]);


                if (0 == (((((O << u) >> 1) + ((ssbIndex * M) >> 1)) / N_frame_slot[u]) % 2))
                {
                    printf("frame[ even ]\n");
                }
                else
                {
                    printf("frame[ odd ]\n");
                }
                printf("  + slot[ %d ]\n", n0);
                printf("      + symbol[ ");
                j = symb1st;
                for (i=0; i<pResource->symbNum; i++)
                {
                    printf("%d", j++);
                    if (i<(pResource->symbNum-1)) printf(", ");
                }
                printf(" ]\n");
                printf("  + slot[ %d ]\n", (n0 + 1));
                printf("      + symbol[ ");
                j = symb1st;
                for (i=0; i<pResource->symbNum; i++)
                {
                    printf("%d", j++);
                    if (i<(pResource->symbNum-1)) printf(", ");
                }
                printf(" ]\n");
                printf("\n");
            }
        }
        else if (2 == pResource->pattern)
        {
            if ((ssbSCS == 120) && (pdcchSCS == 60))
            {
                pPattern = Table_13_13(cssId, ssbIndex, SFN_SSB_i, n_SSB_i);
            }
            else if ((ssbSCS == 240) && (pdcchSCS == 120))
            {
                pPattern = Table_13_14(cssId, ssbIndex, SFN_SSB_i, n_SSB_i);
            }

            if ( pPattern )
            {
                printf("frame[ %d ]\n", pPattern->SFNc);
                printf("  + slot[ %d ]\n", pPattern->nc);
                printf("      + symbol[ ");
                j = pPattern->symb1st;
                for (i=0; i<pResource->symbNum; i++)
                {
                    printf("%d", j++);
                    if (i<(pResource->symbNum-1)) printf(", ");
                }
                printf(" ]\n");
                printf("\n");
            }
        }
        else if (3 == pResource->pattern)
        {
            if ((ssbSCS == 120) && (pdcchSCS == 120))
            {
                pPattern = Table_13_15(cssId, ssbIndex, SFN_SSB_i, n_SSB_i);
            }

            if ( pPattern )
            {
                printf("frame[ %d ]\n", pPattern->SFNc);
                printf("  + slot[ %d ]\n", pPattern->nc);
                printf("      + symbol[ ");
                j = pPattern->symb1st;
                for (i=0; i<pResource->symbNum; i++)
                {
                    printf("%d", j++);
                    if (i<(pResource->symbNum-1)) printf(", ");
                }
                printf(" ]\n");
                printf("\n");
            }
        }
    }

    return 0;
}

