#include "const.h"

const SDL_Color COLOR_BG = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_LINES = { 0x67, 0x72, 0x78, 0xFF };
const SDL_Color COLOR_SELECT_1 = { 0xAB, 0x96, 0x44, 0xFF };
const SDL_Color COLOR_SELECT_2 = { 0xAB, 0x44, 0x44, 0xFF };
const SDL_Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Color COLOR_PANE_BG = { 0x00, 0x00, 0x00, 0xDD };
const SDL_Color COLOR_PANE_BG_2 = { 0x30, 0x30, 0x30, 0xDD };
const SDL_Color COLOR_BUTTON_1 = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_BUTTON_2 = { 0x7A, 0x6F, 0x46, 0xFF };
const SDL_Color COLOR_BUTTON_3 = { 0x7A, 0x46, 0x46, 0xFF };

const float KEYB_SPACING_RATIO = 1.0 / 8.0;

const float KEYB_MARGIN_RATIO = 0.2;

const float KEYB_TAB_HEIGHT_RATIO = 0.7;

const float KEYB_TAB_WIDTH_RATIO = 1.7;

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
        case INSTR_BRIDGE: return '#';
        case INSTR_RND: return '?';
        case INSTR_STOP: return '@';

        case INSTR_ADD: return '+';
        case INSTR_SUB: return '-';
        case INSTR_MUL: return '*';
        case INSTR_INTDIV: return '/';
        case INSTR_MOD: return '%';
        case INSTR_NOT: return '!';
        case INSTR_GT: return '`';
        case INSTR_LT: return ',';

        case INSTR_DUP: return ':';
        case INSTR_SWP: return '\\';
        case INSTR_POP: return '$';
        case INSTR_HIF: return '_';
        case INSTR_VIF: return '|';
        case INSTR_STR: return '"';
        case INSTR_ITER: return 'k';

        case INSTR_INTIN: return '&';
        case INSTR_CHARIN: return '~';
        case INSTR_INTOUT: return '.';
        case INSTR_CHAROUT: return ',';

        case INSTR_GET: return 'g';
        case INSTR_PUT: return 'p';
        case INSTR_FETCH: return '\'';
        case INSTR_STORE: return 's';

        default: SDL_Log("Invalid instruction %d", id);
                 return 0;
    }
}

enum INSTR_ID const_befunge_from_char(char c)
{
    switch (c)
    {
        case ' ': return INSTR_SPACE;

        case '0': return INSTR_0;
        case '1': return INSTR_1;
        case '2': return INSTR_2;
        case '3': return INSTR_3;
        case '4': return INSTR_4;
        case '5': return INSTR_5;
        case '6': return INSTR_6;
        case '7': return INSTR_7;
        case '8': return INSTR_8;
        case '9': return INSTR_9;
        case 'a': return INSTR_A;
        case 'b': return INSTR_B;
        case 'c': return INSTR_C;
        case 'd': return INSTR_D;
        case 'e': return INSTR_E;
        case 'f': return INSTR_F;

        case '^': return INSTR_UP;
        case 'v': return INSTR_DOWN;
        case '<': return INSTR_LEFT;
        case '>': return INSTR_RIGHT;
        case '#': return INSTR_BRIDGE;
        case '?': return INSTR_RND;
        case '@': return INSTR_STOP;

        case '+': return INSTR_ADD;
        case '-': return INSTR_SUB;
        case '*': return INSTR_MUL;
        case '/': return INSTR_INTDIV;
        case '%': return INSTR_MOD;
        case '!': return INSTR_NOT;
        case '`': return INSTR_GT;

        case ':': return INSTR_DUP;
        case '\\': return INSTR_SWP;
        case '$': return INSTR_POP;
        case '_': return INSTR_HIF;
        case '|': return INSTR_VIF;
        case '"': return INSTR_STR;
        case 'k': return INSTR_ITER;

        case '&': return INSTR_INTIN;
        case '~': return INSTR_CHARIN;
        case '.': return INSTR_INTOUT;
        case ',': return INSTR_CHAROUT;

        case 'g': return INSTR_GET;
        case 'p': return INSTR_PUT;
        case '\'': return INSTR_FETCH;
        case 's': return INSTR_STORE;

        default: SDL_Log("Invalid instruction character \"%c\"", c);
                 return INSTR_NULL;
    }
}

/*
char *const_get_save_path(char *filename)
{
#ifdef __ANDROID__
    // TODO: Android
#else
    char *prefix = NULL;
    prefix = SDL_GetPrefPath("mbernardi", "sdl_fungeoid");
    if (prefix == NULL)
    {
        SDL_Log("Could not SDL_GetPrefPath");
        return NULL
    }



#endif

}
*/
