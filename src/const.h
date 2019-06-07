#ifndef CONST_H
#define CONST_H

#include "juan.h"
/// Can't include anything else to avoid circular dependencies!

/**
 * Constants manager.
 *
 * Keeps track of global constants, hardcoded values.
 */

// So I can ignore unused arguments
#define UNUSED(x) ((x)=(x))

extern const SDL_Color COLOR_BG;
extern const SDL_Color COLOR_LINES;
extern const SDL_Color COLOR_SELECT_1;
extern const SDL_Color COLOR_SELECT_2;
extern const SDL_Color COLOR_WHITE;
extern const SDL_Color COLOR_PANE_BG;
extern const SDL_Color COLOR_BUTTON_1;
extern const SDL_Color COLOR_BUTTON_2;
extern const SDL_Color COLOR_BUTTON_3;

/// IDs of every instruction
enum INSTR_ID
{
    INSTR_NULL, // Invalid instruction
    INSTR_SPACE,

    INSTR_0,
    INSTR_1,
    INSTR_2,
    INSTR_3,
    INSTR_4,
    INSTR_5,
    INSTR_6,
    INSTR_7,
    INSTR_8,
    INSTR_9,
    INSTR_A,
    INSTR_B,
    INSTR_C,
    INSTR_D,
    INSTR_E,
    INSTR_F,

    INSTR_UP,
    INSTR_DOWN,
    INSTR_LEFT,
    INSTR_RIGHT,
    INSTR_BRIDGE,
    INSTR_RND,
    INSTR_STOP,

    INSTR_ADD,
    INSTR_SUB,
    INSTR_MUL,
    INSTR_INTDIV,
    INSTR_MOD,
    INSTR_NOT,
    INSTR_GT,
    INSTR_LT,

    INSTR_DUP,
    INSTR_SWP,
    INSTR_POP,

    INSTR_HIF,
    INSTR_VIF,
    INSTR_STR,
    INSTR_ITER,

    INSTR_INTIN,
    INSTR_CHARIN,
    INSTR_INTOUT,
    INSTR_CHAROUT,

    INSTR_GET,
    INSTR_PUT,
    INSTR_FETCH,
    INSTR_STORE,

    INSTR_ID_TOTAL,
};

/// Themes for textures of instructions
enum INSTR_THEME
{
    INSTR_THEME_BEFUNGE_CHAR,
    INSTR_THEME_TOTAL
};

/// Return char representing the instruction on Befunge
/*
 * Return 0 if invalid ID.
 */
char const_befunge_char(enum INSTR_ID id);

/// Return INSTR_ID from a char representing a befunge instruction
/*
 * Return INSTR_NULL if invalid ID.
 */
enum INSTR_ID const_befunge_id(char c);

#endif
