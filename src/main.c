#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/* #include <SDL2/SDL.h> */
/* #include <SDL2/SDL_image.h> */
/* #include <SDL2/SDL_ttf.h> */

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

const SDL_Color COLOR_BG = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_LINES = { 0x67, 0x72, 0x78, 0xFF };
const SDL_Color COLOR_SELECT_1 = { 0xAB, 0x96, 0x44, 0xFF };
const SDL_Color COLOR_SELECT_2 = { 0xAB, 0x44, 0x44, 0xFF };
const SDL_Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

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
        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        juan_set_render_draw_color(renderer, &COLOR_LINES);
        for (int i = 0; i < 10; i++)
        {
            SDL_Rect rect = { i * 64 - 2, 0, 2, WINDOW_HEIGHT };
            SDL_RenderFillRect(renderer, &rect);
            SDL_Rect rect2 = { 0, i * 64 - 2, WINDOW_WIDTH, 2 };
            SDL_RenderFillRect(renderer, &rect2);
        }

        juan_set_render_draw_color(renderer, &COLOR_SELECT_1);
        SDL_Rect rect = { 10, 10, 400, 10 };
        SDL_RenderFillRect(renderer, &rect);

        int w = 0;
        int h = 0;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        SDL_Rect r = { 64, 64, w/2, h/2 };
        SDL_RenderCopy(renderer, tex, NULL, &r);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char *argv[])
{
    SDL_Log("sdl_fungeoid starting")

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
