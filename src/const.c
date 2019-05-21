#include "const.h"

char const_befunge_char(enum INSTR_ID id)
{
    switch (id)
    {
        case INSTR_SPACE: return ' ';
        case INSTR_A: return 'a';
        case INSTR_B: return 'b';
        case INSTR_C: return 'c';
        case INSTR_D: return 'd';
        case INSTR_E: return 'e';
        case INSTR_F: return 'f';
        default: return 0;
    }
}
