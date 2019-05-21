#include "canvas.h"

#include <stdlib.h>

/// Pointer to malloc of canvas.
/**
 * Consists of integers that represent values of enum INSTR_ID.
 *
 * The first integer represents (x, y) = (0, 0), the second one (1, 0), the
 * third (2, 0), etc.
 */
static int* CANVAS = NULL;
static int CANVAS_WIDTH = 0;
static int CANVAS_HEIGHT = 0;

int canvas_init(int width, int height)
{
    CANVAS = (int*) malloc(sizeof(int) * width * height);
    if (CANVAS == NULL)
    {
        CANVAS_WIDTH = 0;
        CANVAS_HEIGHT = 0;
        SDL_Log("Failed to malloc canvas\n");
        return 1;
    }
    else
    {
        CANVAS_WIDTH = width;
        CANVAS_HEIGHT = height;
        for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++)
        {
            CANVAS[i] = INSTR_SPACE;
        }
        return 0;
    }
}

int canvas_free()
{
    CANVAS_WIDTH = 0;
    CANVAS_HEIGHT = 0;

    if (CANVAS != NULL)
    {
        free(CANVAS);
    }
    else
    {
        SDL_Log("Tried to free NULL canvas\n");
        return 1;
    }
    return 0;
}

enum INSTR_ID canvas_get_instr(int x, int y)
{
    if (CANVAS == NULL)
    {
        SDL_Log("Tried to use NULL canvas\n");
        return INSTR_NULL;
    }
    if (x >= CANVAS_WIDTH)
    {
        SDL_Log("x position %d out of canvas width %d\n", x, CANVAS_WIDTH);
        return INSTR_NULL;
    }
    if (y >= CANVAS_HEIGHT)
    {
        SDL_Log("y position %d out of canvas height %d\n", y, CANVAS_HEIGHT);
        return INSTR_NULL;
    }

    int memory_position = y * CANVAS_WIDTH + x;
    return CANVAS[memory_position];
}

int canvas_set_instr(int x, int y, enum INSTR_ID id)
{
    if (CANVAS == NULL)
    {
        SDL_Log("Tried to use NULL canvas\n");
        return 1;
    }
    if (x >= CANVAS_WIDTH)
    {
        SDL_Log("x position %d out of canvas width %d\n", x, CANVAS_WIDTH);
        return 1;
    }
    if (y >= CANVAS_HEIGHT)
    {
        SDL_Log("y position %d out of canvas height %d\n", y, CANVAS_HEIGHT);
        return 1;
    }

    int memory_position = y * CANVAS_WIDTH + x;
    CANVAS[memory_position] = id;
    return 0;
}
