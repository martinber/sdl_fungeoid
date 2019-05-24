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
static int BUTTON_SIZE = 96;
#else
static int CELL_SIZE = 32;
static int BUTTON_SIZE = 64;
#endif
// Window dimensions, initial values are used for desktop, when on Android or
// resizing these values will be updated
static int WINDOW_WIDTH = 64 * 10;
static int WINDOW_HEIGHT = 64 * 14;
static const char WINDOW_TITLE[] = "sdl_fungeoid";


static void main_loop(SDL_Renderer *renderer)
{
    bool running = true;
    SDL_Event event;

    SDL_Point screen_size = { WINDOW_WIDTH, WINDOW_HEIGHT };
    Field field = field_create(10, 14, &screen_size, CELL_SIZE);

    Keyboard keyb = keyb_create(WINDOW_WIDTH, WINDOW_HEIGHT, BUTTON_SIZE);
    InputHandler input = input_create();

    while (running)
    {
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
                        WINDOW_WIDTH = event.window.data1;
                        WINDOW_HEIGHT = event.window.data2;
                        SDL_Point screen_size = { WINDOW_WIDTH, WINDOW_HEIGHT };
                        field_resize_screen(&field, &screen_size, CELL_SIZE);
                        keyb_update_geometry(&keyb, WINDOW_WIDTH, WINDOW_HEIGHT, BUTTON_SIZE);
                        break;
                }
            }

            SDL_Point window_size = { WINDOW_WIDTH, WINDOW_HEIGHT };
            Input i = input_handle_event(&input, &window_size, &event);
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
            }
        }
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
