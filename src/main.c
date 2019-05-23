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

#ifdef __ANDROID__
static int CELL_SIZE = 128;
#else
static int CELL_SIZE = 64;
#endif
// Window dimensions, initial values are used for desktop, when on Android or
// resizing these values will be updated
static int WINDOW_WIDTH = 64 * 10;
static int WINDOW_HEIGHT = 64 * 14;
static const char WINDOW_TITLE[] = "sdl_fungeoid";

/// Draw horizontal line with square cap.
/**
 * Width should be even.
 *
 * The line has a square cap on both ends, so the line is actually width pixels
 * longer than the given length.
 */
static void draw_h_line_cap
(
    SDL_Renderer *renderer,
    int x,
    int y,
    int length,
    int width
) {
    SDL_Rect rect =
    {
        x - width / 2,
        y - width / 2,
        length + width,
        width,
    };
    SDL_RenderFillRect(renderer, &rect);
}

/// Draw vertical line with square cap.
/**
 * Width should be even.
 *
 * The line has a square cap on both ends, so the line is actually width pixels
 * longer than the given length.
 */
static void draw_v_line_cap
(
    SDL_Renderer *renderer,
    int x,
    int y,
    int length,
    int width
) {
    SDL_Rect rect =
    {
        x - width / 2,
        y - width / 2,
        width,
        length + width,
    };
    SDL_RenderFillRect(renderer, &rect);
}

static void draw_canvas(SDL_Renderer *renderer, int width, int height)
{
    // Draw lines
    juan_set_render_draw_color(renderer, &COLOR_LINES);
    int line_width = CELL_SIZE / 20;
    for (int x = 0; x < width + 1; x++)
    {
        draw_v_line_cap(renderer, x * CELL_SIZE, 0, height * CELL_SIZE, line_width);
    }
    for (int y = 0; y < height + 1; y++)
    {
        draw_h_line_cap(renderer, 0, y * CELL_SIZE, width * CELL_SIZE, line_width);
    }

    // Draw instructions
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            enum INSTR_ID id = canvas_get_instr(x, y);
            if (id != INSTR_NULL && id != INSTR_SPACE)
            {
                SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, id);
                SDL_Rect r = { x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE };
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
    canvas_set_instr(4, 13, INSTR_CHAROUT);
    canvas_set_instr(9, 13, INSTR_POP);
    int ip_x = 4;
    int ip_y = 1;

    Keyboard keyb = keyb_create(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
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
                        keyb_update_geometry(&keyb, WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
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
                        if (event.type == KEYB_EVENT_ADD_INSTR)
                        {
                            canvas_set_instr(ip_x, ip_y, event.instr_id);
                        }
                        if (event.type == KEYB_EVENT_RM_INSTR)
                        {
                            canvas_set_instr(ip_x, ip_y, INSTR_SPACE);
                        }
                    }
                    else
                    {
                        ip_x = i.point.x / CELL_SIZE;
                        ip_y = i.point.y / CELL_SIZE;
                    }
                case (INPUT_CLICK_MOVE):
                    if (SDL_PointInRect(&i.down_point, &keyb.geometry))
                    {
                        KeyboardEvent event = keyb_handle_input(&keyb, &i);
                    }
            }
        }
        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        draw_canvas(renderer, canvas_width, canvas_height);

        // Draw IP
        int ip_width = CELL_SIZE / 8;
        juan_set_render_draw_color(renderer, &COLOR_SELECT_1);
        draw_h_line_cap(renderer, ip_x * CELL_SIZE, ip_y * CELL_SIZE, CELL_SIZE, ip_width);
        draw_h_line_cap(renderer, ip_x * CELL_SIZE, (ip_y + 1) * CELL_SIZE, CELL_SIZE, ip_width);
        draw_v_line_cap(renderer, ip_x * CELL_SIZE, ip_y * CELL_SIZE, CELL_SIZE, ip_width);
        draw_v_line_cap(renderer, (ip_x + 1) * CELL_SIZE, ip_y * CELL_SIZE, CELL_SIZE, ip_width);

        keyb_draw(renderer, &keyb);

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
