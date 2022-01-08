#ifndef CANVAS_H
#define CANVAS_H

#include "const.h"

/// Array of chars
/**
 * The canvas has integer coordinates, x increases left to right and y increases
 * top to bottom.
 */
typedef struct Canvas
{
    int _width;
    int _height;

    /// Pointer to malloc of canvas.
    /**
     * Consists of chars, most normally is converted after to values of enum
     * INSTR_ID.
     *
     * The first char represents (x, y) = (0, 0), the second one (1, 0), the
     * third (2, 0), etc.
     */
    char *_matrix;
} Canvas;

/// Create canvas
/**
 * Must be freed.
 *
 * On error returns NULL.
 *
 * TODO: Do not ask for width and input as parameters, the canvas should be
 * resized automatically
 */
Canvas *canvas_create(int width, int height);

void canvas_free(Canvas *canvas);

int canvas_get_width(Canvas *canvas);

int canvas_get_height(Canvas *canvas);

/// Get character on given position
/**
 * Returns -1 on error. Result can be casted to char otherwise.
 */
int canvas_get_char(Canvas *canvas, int x, int y);

/// Set character on given position
/**
 * Returns 0 on success.
 */
int canvas_set_char(Canvas *canvas, int x, int y, char c);

/// Save to file
/**
 * Returns 0 on success.
 */
int canvas_save(Canvas *canvas, char *filename);

/// Load from file
/**
 * Returns 0 on success.
 */
int canvas_load(Canvas *canvas, char *filename);

#endif
