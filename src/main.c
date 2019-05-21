#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "const.h"
#include "juan.h"
#include "res.h"
#include "canvas.h"

static const int WINDOW_WIDTH = 600;
static const int WINDOW_HEIGHT = 400;
static const char WINDOW_TITLE[] = "sdl_fungeoid";

const SDL_Color COLOR_BG = { 0x2E, 0x43, 0x4e, 0xFF };
const SDL_Color COLOR_LINES = { 0x67, 0x72, 0x78, 0xFF };
const SDL_Color COLOR_SELECT_1 = { 0xAB, 0x96, 0x44, 0xFF };
const SDL_Color COLOR_SELECT_2 = { 0xAB, 0x44, 0x44, 0xFF };
const SDL_Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

static void draw_canvas(SDL_Renderer *renderer, int width, int height)
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            enum INSTR_ID id = canvas_get_instr(x, y);
            if (id != INSTR_NULL && id != INSTR_SPACE)
            {
                int w;
                int h;
                SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, id);
                SDL_QueryTexture(tex, NULL, NULL, &w, &h);
                SDL_Rect r = { x*64, y*64, w/2, h/2 };
                SDL_RenderCopy(renderer, tex, NULL, &r);
            }
        }
    }
}

static void main_loop(SDL_Renderer *renderer)
{
    bool running = true;
    SDL_Event event;

    int canvas_width = 10;
    int canvas_height = 14;
    if (canvas_init(canvas_width, canvas_height) != 0) {
        return;
    }
    canvas_set_instr(0, 0, INSTR_A);
    canvas_set_instr(1, 0, INSTR_B);
    canvas_set_instr(0, 1, INSTR_C);
    canvas_set_instr(9, 6, INSTR_D);
    canvas_set_instr(4, 13, INSTR_E);
    canvas_set_instr(9, 13, INSTR_F);
    canvas_set_instr(1, 1, INSTR_A);
    canvas_set_instr(1, 1, INSTR_SPACE);

    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        /* x = 100; */
                        break;
                    case SDLK_AC_BACK:
                        running = false;
                        break;
                }
            }
            else if (event.type == SDL_FINGERDOWN)
            {
                /* x = event.tfinger.x * WINDOW_WIDTH; */
            }
            else if (event.type == SDL_FINGERMOTION)
            {
                /* x = event.tfinger.x * WINDOW_WIDTH; */
            }
            else if (event.type == SDL_FINGERUP)
            {
                /* x = 0; */
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

        draw_canvas(renderer, canvas_width, canvas_height);

        SDL_RenderPresent(renderer);
    }

    canvas_free();
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    SDL_Log("sdl_fungeoid starting");

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (juan_init(&window, &renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) != 0) {
        printf("Error creating window or renderer\n");
        return 1;
    }

    if (res_load_all(renderer) == 0)
    {
        main_loop(renderer);
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
