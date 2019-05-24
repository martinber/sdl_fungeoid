#include "canvas.h"

#include <stdlib.h>


Canvas canvas_init(int width, int height)
{
    Canvas canvas =
    {
        .width = width,
        .height = height,

        .matrix = NULL,
    };

    canvas.matrix = (int*) malloc(sizeof(int) * width * height);
    if (canvas.matrix == NULL)
    {
        canvas.width = 0;
        canvas.height = 0;
        SDL_Log("Failed to malloc canvas\n");
    }
    else
    {
        for (int i = 0; i < canvas.width * canvas.height; i++)
        {
            canvas.matrix[i] = INSTR_SPACE;
        }
    }
    return canvas;
}

int canvas_free(Canvas *canvas)
{
    canvas->width = 0;
    canvas->width = 0;

    if (canvas->matrix != NULL)
    {
        free(canvas->matrix);
    }
    else
    {
        SDL_Log("Tried to free NULL canvas\n");
        return 1;
    }
    return 0;
}

enum INSTR_ID canvas_get_instr(Canvas *canvas, int x, int y)
{
    if (canvas->matrix == NULL)
    {
        SDL_Log("Tried to use NULL canvas\n");
        return INSTR_NULL;
    }
    if (x >= canvas->width)
    {
        SDL_Log("x position %d out of canvas width %d\n", x, canvas->width);
        return INSTR_NULL;
    }
    if (y >= canvas->height)
    {
        SDL_Log("y position %d out of canvas height %d\n", y, canvas->height);
        return INSTR_NULL;
    }

    int memory_position = y * canvas->width + x;
    return canvas->matrix[memory_position];
}

int canvas_set_instr(Canvas *canvas, int x, int y, enum INSTR_ID id)
{
    if (canvas->matrix == NULL)
    {
        SDL_Log("Tried to use NULL canvas\n");
        return 1;
    }
    if (x >= canvas->width || x < 0)
    {
        SDL_Log("x position %d out of bounds", x);
        return 1;
    }
    if (y >= canvas->height || y < 0)
    {
        SDL_Log("y position %d out of bounds", y);
        return 1;
    }

    int memory_position = y * canvas->width + x;
    canvas->matrix[memory_position] = id;
    return 0;
}
