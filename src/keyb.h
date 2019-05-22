#ifndef KEYB_H
#define KEYB_H

#include "juan.h"

/**
 * On-screen keyboard
 */

typedef struct Keyboard
{
    SDL_Rect geometry;
    int button_size;
} Keyboard;

/// Create keyb
Keyboard keyb_create(int window_width, int window_height, int button_size);

/// Update size and position from window and cell size
void keyb_update_geometry
(
    Keyboard *keyb,
    int window_width,
    int window_height,
    int button_size
);

/// Get position
void keyb_get_pos(Keyboard *keyb, int *x, int *y);

/// Get size
void keyb_get_size(Keyboard *keyb, int *width, int *height);

/// Draw keyb
void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb);

/// Handle fingerdown event
void keyb_handle_fingerdown
(
    Keyboard *keyb,
    int x,
    int y
);

/// Handle fingermotion event
void keyb_handle_fingermotion
(
    Keyboard *keyb,
    int x,
    int y
);

/// Handle fingerup event
void keyb_handle_fingerup
(
    Keyboard *keyb,
    int x,
    int y
);

#endif
