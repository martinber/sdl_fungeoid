#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "const.h"
#include "juan.h"
#include "res.h"
#include "canvas.h"
#include "keyb.h"
#include "input.h"
#include "field.h"

#ifdef __ANDROID__
static int CELL_SIZE = 64;
static int BUTTON_SIZE = 128;
#else
static int CELL_SIZE = 32;
static int BUTTON_SIZE = 64;
#endif
// Window dimensions, initial values are used for desktop, when on Android or
// resizing these values will be updated
static SDL_Point WINDOW_SIZE = { 64 * 10, 64 * 14 };
static const char WINDOW_TITLE[] = "sdl_fungeoid";

static void main_loop(SDL_Renderer *renderer)
{
    Field field = field_create(10, 14, &WINDOW_SIZE, CELL_SIZE);

    Keyboard keyb = keyb_create(WINDOW_SIZE, BUTTON_SIZE);
    InputHandler input = input_create();

    // Time in milliseconds since start of the game
    Uint32 time_abs_ms = SDL_GetTicks();
    // Time in milliseconds since last loop
    Uint32 time_rel_ms = time_abs_ms;

    bool running = true;
    SDL_Event event;
    while (running)
    {
        time_rel_ms = SDL_GetTicks() - time_abs_ms;
        time_abs_ms = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        WINDOW_SIZE.x = event.window.data1;
                        WINDOW_SIZE.y = event.window.data2;
                        field_resize_screen(&field, &WINDOW_SIZE, CELL_SIZE);
                        keyb_update_geometry(&keyb, WINDOW_SIZE, BUTTON_SIZE);
                        break;
                }
            }

            Input i = input_handle_event(&input, &WINDOW_SIZE, &event);
            switch (i.type)
            {
                case (INPUT_CLICK_UP):
                    if (SDL_PointInRect(&i.point, &keyb.geometry))
                    {
                        KeyboardEvent event = keyb_handle_input(&keyb, &i);
                        field_handle_keyb(&field, &event);
                    }
                    else
                    {
                        field_handle_input(&field, &i);
                    }
                    break;
                case (INPUT_CLICK_MOVE):
                    if (SDL_PointInRect(&i.down_point, &keyb.geometry))
                    {
                        KeyboardEvent event = keyb_handle_input(&keyb, &i);
                        field_handle_keyb(&field, &event);
                    }
                    else
                    {
                        field_handle_input(&field, &i);
                    }
                    break;
                default:
                    break;
            }
        }
        field_update(&field, time_abs_ms);

        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        field_draw(renderer, &field);
        keyb_draw(renderer, &keyb);

        SDL_RenderPresent(renderer);
    }

    field_free(&field);
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    SDL_Log("sdl_fungeoid starting");

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (juan_init(&window, &renderer, WINDOW_TITLE, WINDOW_SIZE.x, WINDOW_SIZE.y) != 0) {
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
