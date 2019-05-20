#include "res.h"

#include "juan.h"

TTF_Font* font_128 = NULL;

int res_load_all(SDL_Renderer *renderer)
{
    RES_TEXTURES[RES_TEX_BALL] = juan_load_texture(renderer, "res/test.png");
    if (RES_TEXTURES[RES_TEX_BALL] == NULL) {
        // Remember to free succesfully loaded textures
        return 1;
    }

    font_128 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 128);
    if (font_128 == NULL)
    {
        SDL_DestroyTexture(RES_TEXTURES[RES_TEX_BALL]);
        RES_TEXTURES[RES_TEX_BALL] = NULL;

        return 1;
    }

    SDL_Color color = { 255, 255, 255, 255 };
    RES_TEXTURES[RES_TEX_CHAR_G] = juan_text_texture(renderer, font_128, "g", color);
    if (RES_TEXTURES[RES_TEX_CHAR_G] == NULL)
    {
        SDL_DestroyTexture(RES_TEXTURES[RES_TEX_BALL]);
        RES_TEXTURES[RES_TEX_BALL] = NULL;

        TTF_CloseFont(font_128);
        font_128 = NULL;

        return 1;
    }

    return 0;
}

void res_free_all()
{
    SDL_DestroyTexture(RES_TEXTURES[RES_TEX_BALL]);
    RES_TEXTURES[RES_TEX_BALL] = NULL;

    TTF_CloseFont(font_128);
    font_128 = NULL;
}
