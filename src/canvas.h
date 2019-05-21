#ifndef CANVAS_H
#define CANVAS_H

#include "const.h"

/**
 * Canvas.
 *
 * Manages the canvas, containing the array of instructions, etc.
 *
 * The canvas has integer coordinates, x increases left to right and y increases
 * top to bottom.
 */

/// Create canvas
/**
 * Returns 0 on success.
 *
 * The canvas must be freed with canvas_free().
 */
int canvas_init(int width, int height);

/// Free canvas memory
int canvas_free();

/// Get instruction on given position
/**
 * Returns INSTR_NULL on error.
 */
enum INSTR_ID canvas_get_instr(int x, int y);

/// Set instruction on given position
/**
 * Returns 0 on success.
 */
int canvas_set_instr(int x, int y, enum INSTR_ID id);

#endif
