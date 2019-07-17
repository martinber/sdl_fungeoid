#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

#include "juan.h"

enum BUTTON_TYPE
{
    BUTTON_KEYB_INSTR,
    BUTTON_KEYB_TAB,
    BUTTON_KEYB_ARROW,
    BUTTON_KEYB_ACTION,
};

typedef struct Button
{
    SDL_Rect geometry;
    bool toggled;
    bool pressed;

    enum BUTTON_TYPE type;
    // To be filled with enum values of any enum type. Can be zero if unused.
    int id;
} Button;

void button_init(Button *button, enum BUTTON_TYPE type, int id);

#endif
