#include "res.h"

#include "juan.h"

static TTF_Font* font_128 = NULL;
static TTF_Font* font_90 = NULL;
static TTF_Font* font_32 = NULL;

/// Array of textures for every char, element 0 corresponds to char -128
static SDL_Texture *CHAR_TEXTURES[256] = { NULL };

/// Array of textures for every keyboard icon for the current theme and the current resolution
static SDL_Texture *KEYB_ICON_TEXTURES[RES_KEYB_ICON_ID_TOTAL] = { NULL };

/// Array of textures for every keyboard tab text for the current theme and the current resolution
static SDL_Texture *KEYB_TAB_TEXTURES[RES_KEYB_TAB_ID_TOTAL] = { NULL };

/// Resolution of instruction textures (128x128)
static const int INSTR_TEXTURES_RES = 128;

/// Draw centered text into a fixed size texture
/**
 * Make sure that the text fits into the given rectangle, the size of the text
 * is determined by the font.
 */
static SDL_Texture *tex_from_centered_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    int width,
    int height,
    char* text,
    SDL_Color color
) {
    // Create a empty square surface and draw a letter in the center of it
    // Apparently when rendering text on Blended mode, the surfaces are
    // ARGB32 bit

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat
    (
        0,
        width, height,
        32, SDL_PIXELFORMAT_ARGB32
    );
    if (surface == NULL)
    {
        SDL_Log("Unable to create empty surface for text: %s\n", text);
        SDL_Log("SDL_CreateRGBSurfaceWithFormat Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface* text_surface = TTF_RenderUTF8_Blended(font, text, color);
    if (text_surface == NULL)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to create surface for text: %s\n", text);
        SDL_Log("TTF_RenderText_Blended Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Rect dst_rect =
    {
        width / 2 - text_surface->w / 2,
        height / 2 - text_surface->h / 2,
        0, // width ignored
        0, // height ignored
    };
    int blit_result = SDL_BlitSurface(text_surface, NULL, surface, &dst_rect);
    SDL_FreeSurface(text_surface);
    if (blit_result != 0)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to blit surface for text: %s\n", text);
        SDL_Log("SDL_BlitSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL)
    {
        SDL_Log("Unable to create texture for text: %s\n", text);
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}
static SDL_Texture *instr_tex_from_char(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char character,
    SDL_Color color
) {
    // The text will be simply the character if printable, or caret notation
    // which consists of two characters
    // For negative numbers I add a minus in the front or an underscore which
    // would be a "minus caret notation char"
    char text[3] = { '\0', '\0', '\0' };
    switch (character)
    {
        case 0:  strcpy(text, "^@"); break;
        case 1:  strcpy(text, "^A"); break;
        case 2:  strcpy(text, "^B"); break;
        case 3:  strcpy(text, "^C"); break;
        case 4:  strcpy(text, "^D"); break;
        case 5:  strcpy(text, "^E"); break;
        case 6:  strcpy(text, "^F"); break;
        case 7:  strcpy(text, "^G"); break;
        case 8:  strcpy(text, "^H"); break;
        case 9:  strcpy(text, "^I"); break;
        case 10: strcpy(text, "^J"); break;
        case 11: strcpy(text, "^K"); break;
        case 12: strcpy(text, "^L"); break;
        case 13: strcpy(text, "^M"); break;
        case 14: strcpy(text, "^N"); break;
        case 15: strcpy(text, "^O"); break;
        case 16: strcpy(text, "^P"); break;
        case 17: strcpy(text, "^Q"); break;
        case 18: strcpy(text, "^R"); break;
        case 19: strcpy(text, "^S"); break;
        case 20: strcpy(text, "^T"); break;
        case 21: strcpy(text, "^U"); break;
        case 22: strcpy(text, "^V"); break;
        case 23: strcpy(text, "^W"); break;
        case 24: strcpy(text, "^X"); break;
        case 25: strcpy(text, "^Y"); break;
        case 26: strcpy(text, "^Z"); break;
        case 27: strcpy(text, "^["); break;
        case 28: strcpy(text, "^\\"); break;
        case 29: strcpy(text, "^]"); break;
        case 30: strcpy(text, "^^"); break;
        case 31: strcpy(text, "^_"); break;
        case 127: strcpy(text, "^?"); break;
        case -1:  strcpy(text, "_A"); break;
        case -2:  strcpy(text, "_B"); break;
        case -3:  strcpy(text, "_C"); break;
        case -4:  strcpy(text, "_D"); break;
        case -5:  strcpy(text, "_E"); break;
        case -6:  strcpy(text, "_F"); break;
        case -7:  strcpy(text, "_G"); break;
        case -8:  strcpy(text, "_H"); break;
        case -9:  strcpy(text, "_I"); break;
        case -10: strcpy(text, "_J"); break;
        case -11: strcpy(text, "_K"); break;
        case -12: strcpy(text, "_L"); break;
        case -13: strcpy(text, "_M"); break;
        case -14: strcpy(text, "_N"); break;
        case -15: strcpy(text, "_O"); break;
        case -16: strcpy(text, "_P"); break;
        case -17: strcpy(text, "_Q"); break;
        case -18: strcpy(text, "_R"); break;
        case -19: strcpy(text, "_S"); break;
        case -20: strcpy(text, "_T"); break;
        case -21: strcpy(text, "_U"); break;
        case -22: strcpy(text, "_V"); break;
        case -23: strcpy(text, "_W"); break;
        case -24: strcpy(text, "_X"); break;
        case -25: strcpy(text, "_Y"); break;
        case -26: strcpy(text, "_Z"); break;
        case -27: strcpy(text, "_["); break;
        case -28: strcpy(text, "_\\"); break;
        case -29: strcpy(text, "_]"); break;
        case -30: strcpy(text, "_^"); break;
        case -31: strcpy(text, "__"); break;
        case -127: strcpy(text, "_?"); break;
        case -128: strcpy(text, "_@"); break;

        default:
            if (character > 0) {
                text[0] = character;
            } else {
                text[0] = '-';
                text[1] = -character;
            }

            break;
    }

    return tex_from_centered_text(renderer, font,
            INSTR_TEXTURES_RES, INSTR_TEXTURES_RES, text, color);
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
    font_128 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 110);
    font_90 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 76);
    font_32 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 32);
    if (font_128 == NULL || font_90 == NULL || font_32 == NULL)
    {
        res_free_all();
        return 1;
    }

    // Load every instruction texture for characters

    for (int c = 0; c < 256; c++)
    {
        char character = (char)(c - 128);
        CHAR_TEXTURES[c] = instr_tex_from_char(renderer, font_128, character, COLOR_WHITE);
        if (CHAR_TEXTURES[c] == NULL)
        {
            SDL_Log("Error creating instruction texture for char %d\n", character);
            res_free_all();
            return 1;
        }
    }

    // Load every tab texture from characters
    char *tab_text[RES_KEYB_TAB_ID_TOTAL] =
    {
        "run", "misc", "123", "<>io", "+*/", "char",
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

    // TODO: Decide now whatspacing to use between buttons and update this /8
    // Load every keyboard icon texture
    for (enum RES_KEYB_ICON_ID i = 0; i < RES_KEYB_ICON_ID_TOTAL; i++)
    {
        switch (i)
        {
            case RES_KEYB_ICON_DELETE:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/delete.png");
                break;

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

            case RES_KEYB_ICON_START:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "start", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_STOP:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "stop", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_STEP:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "step", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_RESUME:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "resume", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_TIME_FASTER:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/time_faster.png");
                break;

            case RES_KEYB_ICON_TIME_SLOWER:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/time_slower.png");
                break;

            case RES_KEYB_ICON_NEW:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "new", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_LOAD:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "load", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_SAVE:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "save", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_SAVE_AS:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "saveas", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_T_HELP:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 4 + INSTR_TEXTURES_RES * 3 / 8,
                        INSTR_TEXTURES_RES,
                        "toggle help", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_T_VALUES:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 4 + INSTR_TEXTURES_RES * 3 / 8,
                        INSTR_TEXTURES_RES,
                        "toggle values", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_QUIT:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "quit", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_ZOOM_IN:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/zoom_in.png");
                break;

            case RES_KEYB_ICON_ZOOM_OUT:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/zoom_out.png");
                break;

            case RES_KEYB_ICON_ARROW_UP:
                KEYB_ICON_TEXTURES[i] = juan_load_texture(renderer,
                        "res/keyb_icons/arrow_up.png");
                break;

            case RES_KEYB_ICON_CHAR_GET:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "get", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_PUT:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES * 2 + INSTR_TEXTURES_RES / 8,
                        INSTR_TEXTURES_RES,
                        "put", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_ADD_1:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "+1", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_SUB_1:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "-1", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_ADD_10:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "+10", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_SUB_10:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "-10", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_ADD_16:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "+16", COLOR_WHITE);
                break;

            case RES_KEYB_ICON_CHAR_SUB_16:
                KEYB_ICON_TEXTURES[i] = tex_from_centered_text(renderer, font_90,
                        INSTR_TEXTURES_RES,
                        INSTR_TEXTURES_RES,
                        "-16", COLOR_WHITE);
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

    for (int i = 0; i < 256; i++)
    {
        if (CHAR_TEXTURES[i] != NULL)
        {
            SDL_DestroyTexture(CHAR_TEXTURES[i]);
            CHAR_TEXTURES[i] = NULL;
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
    char c = const_befunge_char(id);
    if (c == 0) {
        SDL_Log("Tried to get instruction texture of ID %d\n", id);
        return NULL;
    }
    return res_get_instr_char_tex(theme, c);
}

SDL_Texture *res_get_instr_char_tex(enum INSTR_THEME theme, char c)
{
    UNUSED(theme);
    return CHAR_TEXTURES[(unsigned char)(c + 128)];
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
