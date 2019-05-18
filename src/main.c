#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "juan.h"

/*
 * Functions return non-zero on error
 *
 * https://www.willusher.io/pages/sdl2/
 */

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const char WINDOW_TITLE[] = "sdl_fungeoid";

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (juan_init(&window, &renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) != 0) {
        printf("Error creating window or renderer\n");
        return 1;
    }

    SDL_Texture *tex = juan_load_texture(renderer, "res/test.png");

    SDL_Delay(100);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
