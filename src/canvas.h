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
typedef struct Canvas
{
    int width;
    int height;

    /// Pointer to malloc of canvas.
    /**
     * Consists of integers that represent values of enum INSTR_ID.
     *
     * The first integer represents (x, y) = (0, 0), the second one (1, 0), the
     * third (2, 0), etc.
     */
    int* matrix;
} Canvas;

/// Create canvas
/**
 * On error canvas.matrix is NULL.
 *
 * The canvas must be freed with canvas_free().
 */
Canvas canvas_init(int width, int height);

/// Free canvas memory
int canvas_free(Canvas *canvas);

/// Get instruction on given position
/**
 * Returns INSTR_NULL on error.
 */
enum INSTR_ID canvas_get_instr(Canvas *canvas, int x, int y);

/// Set instruction on given position
/**
 * Returns 0 on success.
 */
int canvas_set_instr(Canvas *canvas, int x, int y, enum INSTR_ID id);


#endif
