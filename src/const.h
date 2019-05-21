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

/// IDs of every instruction
enum INSTR_ID
{
    INSTR_A,
    INSTR_B,
    INSTR_C,
    INSTR_D,
    INSTR_E,
    INSTR_F,
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

#endif
