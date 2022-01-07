#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "const.h"
#include "juan.h"
#include "res.h"
#include "screens.h"

#include "os.h"

// Initial window dimensions used for desktop
static SDL_Point INITIAL_WINDOW_SIZE = { 64 * 10, 64 * 14 };
static const char WINDOW_TITLE[] = "sdl_fungeoid";

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    SDL_Log("sdl_fungeoid starting");

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if
    (
        juan_init(
                &window, &renderer, WINDOW_TITLE,
                INITIAL_WINDOW_SIZE.x, INITIAL_WINDOW_SIZE.y
                ) != 0
    ) {
        printf("Error creating window or renderer\n");
        return 1;
    }

    // Check the screen size actually used
    SDL_Point window_size = { 0, 0 };
    if (SDL_GetRendererOutputSize(renderer, &window_size.x, &window_size.y) != 0)
    {
        SDL_Log("SDL_GetRendererOutputSize Error: %s\n", SDL_GetError());
    }

    // Init RNG
    srand((unsigned int) time(NULL));

    if (res_load_all(renderer) == 0)
    {
        ScreensHandler *screens = screens_create(window_size);
        if (screens != NULL)
        {
            screens_loop(screens, renderer);
            screens_free(screens);
        }
        res_free_all();
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
