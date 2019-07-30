#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

#include "juan.h"

enum BUTTON_TYPE
{
    BUTTON_KEYB_INSTR, // Instructrions or "normal" buttons
    BUTTON_KEYB_TAB, // Keyboard tabs
    BUTTON_KEYB_ARROW, // Keyboard arrows
    BUTTON_KEYB_ACTION, // Keyboard buttons available on almost every tab
    BUTTON_TYPE_TOTAL,
};

/// Contains geometry, state and ID of a button.
typedef struct Button
{
    SDL_Rect geometry;
    bool toggled;
    bool pressed;

    enum BUTTON_TYPE type;
    // To be filled with enum values of any enum type. Can be zero if unused.
    int id;
} Button;

/// Initialize button values
/**
 * Set a type and an ID, remaining values are set to zero.
 */
void button_init(Button *button, enum BUTTON_TYPE type, int id);

#endif
