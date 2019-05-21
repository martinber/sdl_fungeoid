#include "const.h"

const SDL_Color COLOR_BG = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_LINES = { 0x67, 0x72, 0x78, 0xFF };
const SDL_Color COLOR_SELECT_1 = { 0xAB, 0x96, 0x44, 0xFF };
const SDL_Color COLOR_SELECT_2 = { 0xAB, 0x44, 0x44, 0xFF };
const SDL_Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Color COLOR_PANE_BG = { 0x00, 0x00, 0x00, 0xDD };
const SDL_Color COLOR_BUTTON_1 = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_BUTTON_2 = { 0x7A, 0x6F, 0x46, 0xFF };
const SDL_Color COLOR_BUTTON_3 = { 0x7A, 0x46, 0x46, 0xFF };

char const_befunge_char(enum INSTR_ID id)
{
    switch (id)
    {
        case INSTR_SPACE: return ' ';

        case INSTR_0: return '0';
        case INSTR_1: return '1';
        case INSTR_2: return '2';
        case INSTR_3: return '3';
        case INSTR_4: return '4';
        case INSTR_5: return '5';
        case INSTR_6: return '6';
        case INSTR_7: return '7';
        case INSTR_8: return '8';
        case INSTR_9: return '9';
        case INSTR_A: return 'a';
        case INSTR_B: return 'b';
        case INSTR_C: return 'c';
        case INSTR_D: return 'd';
        case INSTR_E: return 'e';
        case INSTR_F: return 'f';

        case INSTR_UP: return '^';
        case INSTR_DOWN: return 'v';
        case INSTR_LEFT: return '<';
        case INSTR_RIGHT: return '>';

        case INSTR_ADD: return '+';
        case INSTR_SUB: return '-';
        case INSTR_MUL: return '*';
        case INSTR_INTDIV: return '/';

        case INSTR_DUP: return ':';
        case INSTR_SWP: return '\\';
        case INSTR_POP: return '$';

        case INSTR_STR: return '"';

        case INSTR_INTOUT: return '.';
        case INSTR_CHAROUT: return ',';

        default: SDL_Log("Invalid instruction %d", id);
                 return 0;
    }
}
