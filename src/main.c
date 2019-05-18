#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "juan.h"
#include "res.h"

/*
 * Functions return non-zero on error
 *
 * https://www.willusher.io/pages/sdl2/
 */

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const char WINDOW_TITLE[] = "sdl_fungeoid";

void main_loop(SDL_Renderer *renderer, SDL_Texture *tex)
{
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (juan_init(&window, &renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) != 0) {
        printf("Error creating window or renderer\n");
        return 1;
    }

    if (res_load_all(renderer) == 0)
    {
        main_loop(renderer, RES_TEXTURES[RES_TEX_CHAR_G]);
        res_free_all();
    }

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
