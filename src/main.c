#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

/*
 * Functions return non-zero on error
 *
 * https://www.willusher.io/pages/sdl2/
 */

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const char WINDOW_TITLE[] = "sdl_fungeoid";

/*
 * Start SDL and open the window
 */
int init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow
    (
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (win == NULL)
    {
        printf("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    return 0;
}

void main_loop()
{
    bool running = true;

    while (running)
    {
    }
}


int main(int argc, char *argv[])
{

    if (init_sdl() != 0) {
        printf("Error creating window");
        return 1;
    }

    main_loop();

    return 0;
}
