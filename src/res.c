#include "res.h"

#include "juan.h"

static TTF_Font* font_128 = NULL;
static TTF_Font* font_32 = NULL;

/// Array of textures for every instruction for the current theme and the current resolution
/**
 * Pointers for INSTR_NULL and INSTR_SPACE should be NULL
 */
static SDL_Texture *INSTR_TEXTURES[INSTR_ID_TOTAL] = { NULL };

/// Array of textures for every keyboard icon for the current theme and the current resolution
static SDL_Texture *KEYB_ICON_TEXTURES[RES_KEYB_ICON_ID_TOTAL] = { NULL };

/// Array of textures for every keyboard tab text for the current theme and the current resolution
static SDL_Texture *KEYB_TAB_TEXTURES[RES_KEYB_TAB_ID_TOTAL] = { NULL };

/// Resolution of instruction textures (128x128)
static const int INSTR_TEXTURES_RES = 128;

static SDL_Texture *instr_tex_from_char(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char character,
    SDL_Color color
) {
    // Create a empty square surface and draw a letter in the center of it
    // Apparently when rendering text on Blended mode, the surfaces are
    // ARGB32 bit

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat
    (
        0,
        INSTR_TEXTURES_RES, INSTR_TEXTURES_RES,
        32, SDL_PIXELFORMAT_ARGB32
    );
    if (surface == NULL)
    {
        SDL_Log("Unable to create empty surface for char: %c\n", character);
        SDL_Log("SDL_CreateRGBSurfaceWithFormat Error: %s\n", SDL_GetError());
        return NULL;
    }

    char string[2] = { character, '\0' };
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, string, color);
    if (text_surface == NULL)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to create surface for text: %s\n", string);
        SDL_Log("TTF_RenderText_Blended Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Rect dst_rect =
    {
        INSTR_TEXTURES_RES / 2 - text_surface->w / 2,
        0,
        0, // width ignored
        0, // height ignored
    };
    int blit_result = SDL_BlitSurface(text_surface, NULL, surface, &dst_rect);
    SDL_FreeSurface(text_surface);
    if (blit_result != 0)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to blit surface for char: %c\n", character);
        SDL_Log("SDL_BlitSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL)
    {
        SDL_Log("Unable to create texture for char: %c\n", character);
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

int res_load_all(SDL_Renderer *renderer)
{
    /*
    RES_TEXTURES[RES_TEX_BALL] = juan_load_texture(renderer, "res/test.png");
    if (RES_TEXTURES[RES_TEX_BALL] == NULL) {
        // Remember to free succesfully loaded textures
        return 1;
    }
    */
    font_128 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 128);
    font_32 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 32);
    if (font_128 == NULL || font_32 == NULL)
    {
        res_free_all();
        return 1;
    }

    // Load every instruction texture from characters

    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        if (i != INSTR_NULL && i != INSTR_SPACE)
        {
            INSTR_TEXTURES[i] = instr_tex_from_char(
                    renderer, font_128, const_befunge_char(i), COLOR_WHITE);
            if (INSTR_TEXTURES[i] == NULL)
            {
                SDL_Log("Error creating instruction texture for ID %d\n", i);
                res_free_all();
                return 1;
            }
        }
    }

    // Load every tab texture from characters
    char *tab_text[RES_KEYB_TAB_ID_TOTAL] =
    {
        "run", "123", "<>io", "+*/", "char",
    };

    for (enum RES_KEYB_TAB_ID i = 0; i < RES_KEYB_TAB_ID_TOTAL; i++)
    {
        KEYB_TAB_TEXTURES[i] = juan_text_texture(
                renderer, font_128, tab_text[i], COLOR_WHITE);
        if (KEYB_TAB_TEXTURES[i] == NULL)
        {
            SDL_Log("Error creating tab texture for ID %d\n", i);
            res_free_all();
            return 1;
        }
    }

    // Load every keyboard icon texture
    for (enum RES_KEYB_ICON_ID i = 0; i < RES_KEYB_ICON_ID_TOTAL; i++)
    {
        switch (i)
        {
            case RES_KEYB_ICON_SHIFT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/shift.png");
                break;

            case RES_KEYB_ICON_SELECT_RECT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/select_rect.png");
                break;

            case RES_KEYB_ICON_SELECT_PAINT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/select_paint.png");
                break;

            case RES_KEYB_ICON_COPY:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/copy.png");
                break;

            case RES_KEYB_ICON_CUT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/cut.png");
                break;

            case RES_KEYB_ICON_PASTE:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/paste.png");
                break;

            case RES_KEYB_ICON_COMMENT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/comment.png");
                break;

            default:
                SDL_Log("Tried to load unknown keyb_icon texture ID %d\n", i);
                res_free_all();
                return 1;

        }
        if (KEYB_ICON_TEXTURES[i] == NULL)
        {
            SDL_Log("Error loading keyb_icon texture for ID %d\n", i);
            res_free_all();
            return 1;
        }
    }

    return 0;
}

void res_free_all()
{
    //SDL_DestroyTexture(RES_TEXTURES[RES_TEX_BALL]);
    //RES_TEXTURES[RES_TEX_BALL] = NULL;

    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        if (INSTR_TEXTURES[i] != NULL)
        {
            SDL_DestroyTexture(INSTR_TEXTURES[i]);
            INSTR_TEXTURES[i] = NULL;
        }
    }

    for (enum RES_KEYB_ICON_ID i = 0; i < RES_KEYB_ICON_ID_TOTAL; i++)
    {
        if (KEYB_ICON_TEXTURES[i] != NULL)
        {
            SDL_DestroyTexture(KEYB_ICON_TEXTURES[i]);
            KEYB_ICON_TEXTURES[i] = NULL;
        }
    }

    for (enum RES_KEYB_TAB_ID i = 0; i < RES_KEYB_TAB_ID_TOTAL; i++)
    {
        if (KEYB_TAB_TEXTURES[i] != NULL)
        {
            SDL_DestroyTexture(KEYB_TAB_TEXTURES[i]);
            KEYB_TAB_TEXTURES[i] = NULL;
        }
    }

    if (font_128 != NULL)
    {
        TTF_CloseFont(font_128);
        font_128 = NULL;
    }
    if (font_32 != NULL)
    {
        TTF_CloseFont(font_32);
        font_32 = NULL;
    }
}

SDL_Texture *res_get_instr_tex(enum INSTR_THEME theme, enum INSTR_ID id)
{
    UNUSED(theme);
    if (id == INSTR_NULL || id == INSTR_SPACE) {
        SDL_Log("Tried to get instruction texture of ID %d\n", id);
        return NULL;
    }
    return INSTR_TEXTURES[id];
}

SDL_Texture *res_get_keyb_icon_tex(enum INSTR_THEME theme, enum RES_KEYB_ICON_ID id)
{
    UNUSED(theme);
    return KEYB_ICON_TEXTURES[id];
}

SDL_Texture *res_get_keyb_tab_tex(enum INSTR_THEME theme, enum RES_KEYB_TAB_ID id)
{
    UNUSED(theme);
    return KEYB_TAB_TEXTURES[id];
}

TTF_Font *res_get_font(enum RES_FONT_ID font)
{
    switch (font)
    {
        case RES_FONT_STACK:
            return font_32;
            break;
        default:
            SDL_Log("Tried to get font with ID %d\n", font);
            return NULL;
            break;
    }
}
