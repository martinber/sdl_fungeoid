#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "const.h"
#include "juan.h"
#include "res.h"
#include "canvas.h"
#include "keyb.h"
#include "hud.h"
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
    Field *field = field_create(10, 14, &WINDOW_SIZE, CELL_SIZE);
    Keyboard *keyb = keyb_create(WINDOW_SIZE, BUTTON_SIZE);
    Hud *hud = hud_create(WINDOW_SIZE, 32, field_get_stack(field));
    InputHandler *input = input_create();
    if (field == NULL || keyb == NULL || hud == NULL || input == NULL)
    {
        field_free(field);
        field = NULL;
        keyb_free(keyb);
        keyb = NULL;
        hud_free(hud);
        hud = NULL;
        input_free(input);
        input = NULL;
        SDL_Log("Failed to create field, keyb or InputHandler");
        return;
    }

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
                        field_resize_screen(field, &WINDOW_SIZE, CELL_SIZE);
                        keyb_update_geometry(keyb, WINDOW_SIZE, BUTTON_SIZE);
                        hud_update_geometry(hud, WINDOW_SIZE, 32);
                        break;
                }
            }

            Input i = input_handle_event(input, &WINDOW_SIZE, &event);
            KeyboardEvent event = keyb_handle_input(keyb, &i);
            if (event.type == KEYB_EVENT_NOT_HANDLED)
            {
                if (hud_handle_input(hud, &i) != 0)
                {
                    field_handle_input(field, &i);
                }
            }
            else
            {
                field_handle_keyb(field, &event);
            }
        }
        field_update(field, time_abs_ms);

        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        field_draw(renderer, field);
        hud_draw(renderer, hud);
        keyb_draw(renderer, keyb);

        SDL_RenderPresent(renderer);
    }

    field_free(field);
    field = NULL;
    keyb_free(keyb);
    keyb = NULL;
    hud_free(hud);
    hud = NULL;
    input_free(input);
    input = NULL;
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
