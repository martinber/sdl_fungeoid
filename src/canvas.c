#include "canvas.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void canvas_clear(Canvas *canvas)
{
    for (int i = 0; i < canvas->width * canvas->height; i++)
    {
        canvas->matrix[i] = INSTR_SPACE;
    }
}

Canvas *canvas_create(int width, int height)
{
    Canvas *canvas = (Canvas*) malloc(sizeof(Canvas));
    if (canvas == NULL)
    {
        SDL_Log("Failed to malloc Canvas\n");
        return NULL;
    }
    canvas->width = width;
    canvas->height = height;
    canvas->matrix = NULL;

    canvas->matrix = (int*) malloc(sizeof(int) * width * height);
    if (canvas->matrix == NULL)
    {
        free(canvas);
        canvas = NULL;
        SDL_Log("Failed to malloc canvas->matrix\n");
        return NULL;
    }
    canvas_clear(canvas);

    return canvas;
}

void canvas_free(Canvas *canvas)
{
    if (canvas != NULL)
    {
        free(canvas->matrix);
        canvas->matrix = NULL;
    }
    free(canvas);
    canvas = NULL;
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

int canvas_save(Canvas *canvas, char *filename)
{
    if (filename == NULL)
    {
        SDL_Log("Error opening NULL filename");
        return 1;
    }

    // Open file

    FILE *f = NULL;
    errno = 0;
    f = fopen(filename, "w");
    if (f == NULL)
    {
        SDL_Log("Error opening file %s: %s", filename, strerror(errno));
        return 1;
    }

    // Malloc current line to write to file, has size canvas->width + 1 to leave
    // room for \0

    char *line = NULL;
    line = (char*) malloc(sizeof(char) * canvas->width + 1);
    if (line == NULL)
    {
        SDL_Log("Failed to malloc line when saving canvas\n");
        return 1;
    }
    line[canvas->width] = '\0';

    // Iterate over canvas and write to file, ignoring trailing spaces

    for (int y = 0; y < canvas->height; y++)
    {
        // Write instructions to line array
        for (int x = 0; x < canvas->width; x++)
        {
            char c = const_befunge_char(canvas_get_instr(canvas, x, y));
            if (c == 0)
            {
                SDL_Log("Ignoring unknown or null character when saving to file");
                line[x] = const_befunge_char(INSTR_SPACE);
            }
            line[x] = c;
        }
        // Trim trailing spaces
        // Point to last character (excluding the last \0)
        char *end = line + canvas->width - 1;
        while (*end == ' ')
        {
            *end = '\0';
            end--;
        }
        // Write line to file
        fputs(line, f);
        fputc('\n', f);
    }
    // TODO: Maube trim extra lines at the end

    // Close file

    errno = 0;
    if (fclose(f) == EOF)
    {
        SDL_Log("Error closing file %s: %s", filename, strerror(errno));
        return 1;
    }

    return 0;
}

int canvas_load(Canvas *canvas, char *filename)
{
    if (filename == NULL)
    {
        SDL_Log("Error opening NULL filename");
        return 1;
    }

    canvas_clear(canvas);

    // Open file

    FILE *f = NULL;
    errno = 0;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        SDL_Log("Error opening file %s: %s", filename, strerror(errno));
        return 1;
    }

    // Load instructions

    int x = 0;
    int y = 0;
    while (1)
    {
        int c = fgetc(f);
        if (c == EOF)
        {
            if (ferror(f))
            {
                SDL_Log("Error reading file %s at (%d, %d)", filename, x+1, y);
                return 1; // Error reading file
            }
            break; // End of file reached
        }
        if (c == '\n')
        {
            x = 0;
            y += 1;
        }
        else
        {
            if (x >= canvas->width)
            {
                SDL_Log("File too wide to fit on canvas");
            }
            if (y >= canvas->height)
            {
                SDL_Log("File too tall to fit on canvas");
                break; // File too tall
            }
            int id = const_befunge_from_char(c);
            if (id == INSTR_NULL)
            {
                SDL_Log("Ignoring NULL instruction at (%d, %d)", x, y);
            }
            else
            {
                canvas_set_instr(canvas, x, y, id);
            }
            x += 1;
        }
    }

    // Close file

    errno = 0;
    if (fclose(f) == EOF)
    {
        SDL_Log("Error closing file %s: %s", filename, strerror(errno));
        return 1;
    }

    return 0;
}
