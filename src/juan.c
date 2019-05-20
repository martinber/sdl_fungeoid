#include "juan.h"

int juan_init(
    SDL_Window **window,
    SDL_Renderer **renderer,
    const char *title,
    const int width,
    const int height
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

    SDL_Rect screen_rect = { 0, 0, 320, 240 };
    SDL_DisplayMode display_mode;
    if(SDL_GetCurrentDisplayMode(0, &display_mode) == 0)
    {
        screen_rect.w = display_mode.w;
        screen_rect.h = display_mode.h;
    }

    *window = SDL_CreateWindow
    (
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_rect.w, screen_rect.h,
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
