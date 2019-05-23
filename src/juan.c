#include "juan.h"

#include <math.h>
#include <assert.h>
#include <limits.h>

int juan_init(
    SDL_Window **window,
    SDL_Renderer **renderer,
    const char *title,
    int width,
    int height
) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags)
    {
        SDL_Log("IMG_Init Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0){
        SDL_Log("TTL_Init Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

#ifdef __ANDROID__
    // Get phone screen size
    SDL_DisplayMode display_mode;
    if(SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
    {
        SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    width = display_mode.w;
    height = display_mode.h;
#endif

    *window = SDL_CreateWindow
    (
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (*window == NULL)
    {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer
    (
        *window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (*renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Antialiasing for scaling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    return 0;
}

SDL_Texture *juan_load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
    {
        SDL_Log("Unable to load image: %s\n", path);
        SDL_Log("IMG_LoadTexture Error: %s\n", SDL_GetError());
    }
    return texture;
}

TTF_Font *juan_load_font(const char *path, int height)
{
    TTF_Font* font = TTF_OpenFont(path, height);
    if (font == NULL) {
        SDL_Log("Unable to load font: %s\n", path);
        SDL_Log("TTF_OpenFont Error: %s\n", SDL_GetError());
    }
    return font;
}

SDL_Texture *juan_text_texture(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char *text,
    SDL_Color color
) {

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        SDL_Log("Unable to create surface for text: %s\n", text);
        SDL_Log("TTF_RenderText_Blended Error: %s\n", SDL_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        SDL_Log("Unable to create texture for text: %s\n", text);
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }

    return texture;
}

int juan_set_render_draw_color(SDL_Renderer *renderer, const SDL_Color *color) {
    int r = SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    if (r != 0) {
        SDL_Log(
            "Unable to set color: %#x, %#x, %#x, %#x\n",
            color->r, color->g, color->b, color->a
        );
        SDL_Log("SDL_SetRenderDrawColor Error: %s\n", SDL_GetError());
    }
    return r;
}


float juan_point_abs(SDL_Point *a)
{
    return hypotf((float) a->x, (float) a->y);
}

/// Absolute value squared
float juan_point_sqabs(SDL_Point *a)
{
    float x = (float) a->x;
    float y = (float) a->y;
    return x*x + y*y;
}

SDL_Point juan_point_add(SDL_Point *a, SDL_Point *b)
{
    SDL_Point point = { 0, 0 };
    point.x = a->x + b->x;
    point.y = a->y + b->y;
    return point;
}

SDL_Point juan_point_sub(SDL_Point *a, SDL_Point *b)
{
    SDL_Point point = { 0, 0 };
    point.x = a->x - b->x;
    point.y = a->y - b->y;
    return point;
}

/// Distance squared
float juan_point_sq_dist(SDL_Point *a, SDL_Point *b)
{
    SDL_Point sub = juan_point_sub(a, b);
    return juan_point_sqabs(&sub);
}

int juan_ftoi(float x)
{
    assert(x >= INT_MIN-0.5);
    assert(x <= INT_MAX+0.5);
    if (x >= 0)
    {
        return (int) (x+0.5);
    }
    return (int) (x-0.5);
}
