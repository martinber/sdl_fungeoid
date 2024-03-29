#include "keyb.h"

#include "res.h"
#include "os.h"

// List of buttons for each tab

static const enum INSTR_ID KEYB_VALUES_INSTR_ID[KEYB_VALUES_BUTTONS_TOTAL] =
{
    INSTR_0, INSTR_1, INSTR_2, INSTR_3, INSTR_4, INSTR_5, INSTR_6,
    INSTR_7, INSTR_8, INSTR_9,
    INSTR_A, INSTR_B, INSTR_C, INSTR_D, INSTR_E, INSTR_F,
};

static const enum INSTR_ID KEYB_MOVIO_INSTR_ID[KEYB_MOVIO_BUTTONS_TOTAL] =
{
    INSTR_LEFT, INSTR_DOWN, INSTR_UP, INSTR_RIGHT, INSTR_BRIDGE, INSTR_RND,
    INSTR_STOP, INSTR_STR,
    INSTR_INTIN, INSTR_CHARIN, INSTR_INTOUT, INSTR_CHAROUT,
    INSTR_GET, INSTR_PUT, INSTR_FETCH, INSTR_STORE,
};

static const enum INSTR_ID KEYB_OPER_INSTR_ID[KEYB_OPER_BUTTONS_TOTAL] =
{
    INSTR_ADD, INSTR_SUB, INSTR_MUL, INSTR_INTDIV, INSTR_MOD, INSTR_NOT,
    INSTR_GT,
    INSTR_DUP, INSTR_SWP, INSTR_POP, INSTR_HIF, INSTR_VIF, INSTR_STR,
    INSTR_ITER,
};

static void update_curr_char_repr_str(Keyboard *keyb)
{
    char str_buf[32];
    sprintf(str_buf, "% 4d == 0x%02hhX", keyb->curr_char, keyb->curr_char);
    text_set_str(keyb->curr_char_repr, str_buf);
}

Keyboard *keyb_create(SDL_Point window_size)
{
    Keyboard *keyb = (Keyboard*) malloc(sizeof(Keyboard));
    if (keyb == NULL)
    {
        SDL_Log("Failed to malloc Keyboard");
        return NULL;
    }
    keyb->geometry = (SDL_Rect) { 0, 0, 0, 0 };
    keyb->tabs_geometry = (SDL_Rect) { 0, 0, 0, 0 };

    keyb->tabs_offset = 0;
    keyb->tabs_min_offset = 0;

    keyb->active_tab = 0;
    keyb->shift_state = KEYB_SHIFT_NONE;

    keyb->curr_char = '0';

    for (enum KEYB_TAB_ID i = 0; i < KEYB_TAB_ID_TOTAL; i++)
    {
        keyb->tab_geometry[i] = (SDL_Rect) { 0, 0, 0, 0 };
    }

    // Initialize button arrays IDs. The geometries are set to (0,0,0,0)

    for (int i = 0; i < KEYB_VALUES_BUTTONS_TOTAL; i++)
    {
        button_init
        (
            &keyb->values_buttons[i],
            BUTTON_KEYB_INSTR, KEYB_VALUES_INSTR_ID[i]
        );
    }
    for (int i = 0; i < KEYB_MOVIO_BUTTONS_TOTAL; i++)
    {
        button_init
        (
            &keyb->movio_buttons[i],
            BUTTON_KEYB_INSTR, KEYB_MOVIO_INSTR_ID[i]
        );
    }
    for (int i = 0; i < KEYB_OPER_BUTTONS_TOTAL; i++)
    {
        button_init
        (
            &keyb->oper_buttons[i],
            BUTTON_KEYB_INSTR, KEYB_OPER_INSTR_ID[i]
        );
    }
    for (int i = 0; i < KEYB_RUN_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->run_buttons[i], BUTTON_KEYB_INSTR, i);
    }
    for (int i = 0; i < KEYB_MISC_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->misc_buttons[i], BUTTON_KEYB_INSTR, i);
    }
    for (int i = 0; i < KEYB_CHAR_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->char_buttons[i], BUTTON_KEYB_INSTR, i);
    }
    for (int i = 0; i < KEYB_ARROW_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->arrow_buttons[i], BUTTON_KEYB_ARROW, i);
    }
    for (int i = 0; i < KEYB_ACTION_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->action_buttons[i], BUTTON_KEYB_ACTION, i);
    }

    // Texture for the current selected character in the char tab

    keyb->curr_char_repr = text_create(32, res_get_font(RES_FONT_STACK), COLOR_WHITE);
    if (keyb->curr_char_repr == NULL)
    {
        SDL_Log("Error creating text char repr");
        keyb_free(keyb);
        keyb = NULL;
        return NULL;
    }
    keyb->curr_char_repr->h_align = TEXT_ALIGN_CENTER;
    keyb->curr_char_repr->v_align = TEXT_ALIGN_CENTER;

    // Finish

    keyb_update_geometry(keyb, window_size);
    update_curr_char_repr_str(keyb);
    return keyb;
}

void keyb_free(Keyboard *keyb)
{
    if (keyb != NULL)
    {
        text_free(keyb->curr_char_repr);
        keyb->curr_char_repr = NULL;
    }

    free(keyb);
    keyb = NULL;
}

/// Set a SDL_Rect position according to a grid
/**
 * Sets locations of rectangles into a square grid.
 *
 * The grid has:
 * - Cell size (int)
 * - Cell spacing (int)
 * - Origin (SDL_Point)
 *
 * The rect size will be equal to the cell size, the position of the
 * rect will depend on the origin of the cell and the position index
 * (SDL_Point).
 *
 * The pos_index represents an x coordinate and y coordinate. (0, 0) will be
 * located on the grid origin. The coordinates grow to the right and down, can
 * be negative to represent cells left or above the origin.
 */
static void grid_position
(
    SDL_Rect *rect,
    int cell_w,
    int cell_h,
    int cell_spacing,
    int origin_x,
    int origin_y,
    int pos_index_x,
    int pos_index_y
) {

    rect->x = origin_x + pos_index_x * cell_w;
    rect->y = origin_y + pos_index_y * cell_h;
    rect->w = cell_w;
    rect->h = cell_h;

    if (pos_index_x >= 1)
    {
        rect->x += pos_index_x * cell_spacing;
    }
    else if (pos_index_x <= -1)
    {
        rect->x += pos_index_x * cell_spacing;
    }

    if (pos_index_y >= 1)
    {
        rect->y += pos_index_y * cell_spacing;
    }
    else if (pos_index_y <= -1)
    {
        rect->y += pos_index_y * cell_spacing;
    }
}

/// Update the geometry of each button and tab
static void update_buttons(Keyboard *keyb)
{
    int button_size = keyb->button_size;
    int tab_height = button_size * KEYB_TAB_HEIGHT_RATIO;
    int tab_width = button_size * KEYB_TAB_WIDTH_RATIO;
    int tabs_offset = keyb->tabs_offset;

    int margin = button_size * KEYB_MARGIN_RATIO;
    int spacing = button_size * KEYB_SPACING_RATIO;
    int keyb_x = keyb->geometry.x;
    int keyb_y = keyb->geometry.y;
    int keyb_w = keyb->geometry.w;
    int keyb_h = keyb->geometry.h;


    // Tabs
    for (enum KEYB_TAB_ID i = 0; i < KEYB_TAB_ID_TOTAL; i++)
    {
        int origin_x = keyb_x + tabs_offset;
        int origin_y = keyb_y - tab_height;
        grid_position(&keyb->tab_geometry[i], tab_width, tab_height, 0,
                origin_x, origin_y, i, 0);
    }

    keyb->tabs_geometry.w = keyb->tab_geometry[KEYB_TAB_ID_TOTAL - 1].x
            - keyb->tab_geometry[0].x + tab_width;
    keyb->tabs_geometry.h = tab_height;
    keyb->tabs_geometry.x = keyb->tab_geometry[0].x;
    keyb->tabs_geometry.y = keyb->tab_geometry[0].y;

    // Arrow keys
    {
        int origin_x = keyb_x + keyb_w - margin - button_size;
        int origin_y = keyb_y + keyb_h - margin - button_size;
        grid_position(&keyb->arrow_buttons[KEYB_ARROW_RIGHT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->arrow_buttons[KEYB_ARROW_DOWN].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, -1, 0);
        grid_position(&keyb->arrow_buttons[KEYB_ARROW_LEFT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, -2, 0);
        grid_position(&keyb->arrow_buttons[KEYB_ARROW_UP].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, -1, -1);
    }

    // Shift and special buttons
    {
        int origin_x = keyb_x + margin;
        int origin_y = keyb_y + keyb_h - margin - button_size;
        grid_position(&keyb->action_buttons[KEYB_ACTION_DELETE].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, -1);
        grid_position(&keyb->action_buttons[KEYB_ACTION_SHIFT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->action_buttons[KEYB_ACTION_SELECT_RECT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, -1);
        grid_position(&keyb->action_buttons[KEYB_ACTION_SELECT_PAINT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 0);
        grid_position(&keyb->action_buttons[KEYB_ACTION_COPY].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, -1);
        grid_position(&keyb->action_buttons[KEYB_ACTION_CUT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 0);
        grid_position(&keyb->action_buttons[KEYB_ACTION_PASTE].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 3, -1);
        grid_position(&keyb->action_buttons[KEYB_ACTION_COMMENT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 3, 0);
    }

    // MOVIO buttons
    for (int i = 0; i < KEYB_MOVIO_BUTTONS_TOTAL; i++)
    {
        int origin_x = keyb_x + spacing;
        int origin_y = keyb_y + spacing;
        if (i < 8)
        {
            grid_position(&keyb->movio_buttons[i].geometry,
                    button_size, button_size, spacing,
                    origin_x, origin_y, i, 0);
        }
        else
        {
            grid_position(&keyb->movio_buttons[i].geometry,
                    button_size, button_size, spacing,
                    origin_x, origin_y, i - 8, 1);
        }
    }
    // OPER buttons
    for (int i = 0; i < KEYB_OPER_BUTTONS_TOTAL; i++)
    {
        int origin_x = keyb_x + spacing;
        int origin_y = keyb_y + spacing;
        if (i < 7)
        {
            grid_position(&keyb->oper_buttons[i].geometry,
                    button_size, button_size, spacing,
                    origin_x, origin_y, i, 0);
        }
        else
        {
            grid_position(&keyb->oper_buttons[i].geometry,
                    button_size, button_size, spacing,
                    origin_x, origin_y, i - 7, 1);
        }
    }
    // VALUES buttons
    {
        int origin_x = keyb_x + margin + button_size + spacing * 2;
        int origin_y = keyb_y + spacing;
        grid_position(&keyb->values_buttons[7].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->values_buttons[8].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 0);
        grid_position(&keyb->values_buttons[9].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 0);
        grid_position(&keyb->values_buttons[4].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 1);
        grid_position(&keyb->values_buttons[5].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 1);
        grid_position(&keyb->values_buttons[6].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 1);
        grid_position(&keyb->values_buttons[1].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 2);
        grid_position(&keyb->values_buttons[2].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 2);
        grid_position(&keyb->values_buttons[3].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 2);
        grid_position(&keyb->values_buttons[0].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 3);
        grid_position(&keyb->values_buttons[10].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 3, 0);
        grid_position(&keyb->values_buttons[11].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 4, 0);
        grid_position(&keyb->values_buttons[12].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 5, 0);
        grid_position(&keyb->values_buttons[13].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 3, 1);
        grid_position(&keyb->values_buttons[14].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 4, 1);
        grid_position(&keyb->values_buttons[15].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 5, 1);
    }

    // RUN buttons
    {
        int origin_x = keyb_x + spacing;
        int origin_y = keyb_y + spacing;
        grid_position(&keyb->run_buttons[KEYB_RUN_START].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->run_buttons[KEYB_RUN_RESUME].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 1);
        grid_position(&keyb->run_buttons[KEYB_RUN_STOP].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 2);
        grid_position(&keyb->run_buttons[KEYB_RUN_STEP].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 3);
        grid_position(&keyb->run_buttons[KEYB_RUN_FASTER].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 0); // 4 because previous buttons are larger
        grid_position(&keyb->run_buttons[KEYB_RUN_SLOWER].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 1); // 4 because previous buttons are larger
    }

    // MISC buttons
    {
        int origin_x = keyb_x + spacing;
        int origin_y = keyb_y + spacing;
        grid_position(&keyb->misc_buttons[KEYB_MISC_NEW].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->misc_buttons[KEYB_MISC_LOAD].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 0, 1);
        grid_position(&keyb->misc_buttons[KEYB_MISC_SAVE].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 1, 0);
        grid_position(&keyb->misc_buttons[KEYB_MISC_SAVE_AS].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 1, 1);
        grid_position(&keyb->misc_buttons[KEYB_MISC_T_HELP].geometry,
                button_size * 4 + spacing * 3, button_size, spacing,
                origin_x, origin_y, 0, 2);
        grid_position(&keyb->misc_buttons[KEYB_MISC_T_VALUES].geometry,
                button_size * 4 + spacing * 3, button_size, spacing,
                origin_x, origin_y, 0, 3);
        grid_position(&keyb->misc_buttons[KEYB_MISC_QUIT].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 3, 0);
        grid_position(&keyb->misc_buttons[KEYB_MISC_ZOOM_IN].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 5, 0); // 5 because previous buttons are larger
        grid_position(&keyb->misc_buttons[KEYB_MISC_ZOOM_OUT].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 5, 1); // 5 because previous buttons are larger
    }
    // Char buttons
    {
        int origin_x = keyb_x + spacing;
        int origin_y = keyb_y + spacing;
        grid_position(&keyb->char_buttons[KEYB_CHAR_GET].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 3, 0);
        grid_position(&keyb->char_buttons[KEYB_CHAR_PUT].geometry,
                button_size * 2 + spacing, button_size, spacing,
                origin_x, origin_y, 3, 1);

        grid_position(&keyb->char_buttons[KEYB_CHAR_ADD_1].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 0);
        grid_position(&keyb->char_buttons[KEYB_CHAR_SUB_1].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 0, 1);
        grid_position(&keyb->char_buttons[KEYB_CHAR_ADD_10].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 0);
        grid_position(&keyb->char_buttons[KEYB_CHAR_SUB_10].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 1, 1);
        grid_position(&keyb->char_buttons[KEYB_CHAR_ADD_16].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 0);
        grid_position(&keyb->char_buttons[KEYB_CHAR_SUB_16].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 2, 1);

        grid_position(&keyb->char_buttons[KEYB_CHAR_DISPLAY].geometry,
                button_size, button_size, spacing,
                origin_x, origin_y, 4, 1);

        SDL_Rect boundary; // Region where the char_repr should be drawn. The
                           // char_repr will be actually bigger
        grid_position(&boundary,
                button_size, button_size, spacing,
                origin_x, origin_y, 4, 0);
        keyb->curr_char_repr->position.x = (boundary.x + boundary.w / 2);
        keyb->curr_char_repr->position.y = (boundary.y + boundary.h / 2);
    }
}

void keyb_update_geometry(Keyboard *keyb, SDL_Point window_size)
{
    keyb->button_size = juan_min(window_size.x / 9, window_size.y / 12);

    keyb->geometry.w = window_size.x;
    keyb->geometry.h = keyb->button_size * 5.2;
    keyb->geometry.x = 0;
    keyb->geometry.y = window_size.y - keyb->geometry.h;

    update_buttons(keyb);

    keyb->tabs_offset = 0;
    keyb->tabs_min_offset = window_size.x - keyb->tabs_geometry.w;
}

/// Draw a button if it is roughly inside the screen
/**
 * You should set the background color of the button with
 * juan_set_render_draw_color()
 */
static void draw_button
(
    SDL_Renderer *renderer,
    Keyboard *keyb,
    Button *button,
    int keyb_icon_id
) {
    if (button->geometry.x > -keyb->button_size
            && button->geometry.x < keyb->geometry.w + keyb->button_size)
    {
        SDL_RenderFillRect(renderer, &button->geometry);
        SDL_RenderCopy(renderer,
                res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, keyb_icon_id),
                NULL, &button->geometry);
    }
}

void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb)
{
    juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &keyb->geometry);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw tabs

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (enum KEYB_TAB_ID i = 0; i < KEYB_TAB_ID_TOTAL; i++)
    {
        if (i == keyb->active_tab)
        {
            juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
            SDL_RenderFillRect(renderer, &keyb->tab_geometry[i]);
            juan_set_render_draw_color(renderer, &COLOR_SELECT_1);
            SDL_Rect tab_marker =
            {
                .x = keyb->tab_geometry[i].x,
                .y = keyb->tab_geometry[i].y - keyb->tab_geometry[i].h / 10,
                .w = keyb->tab_geometry[i].w,
                .h = keyb->tab_geometry[i].h / 10,
            };
            SDL_RenderFillRect(renderer, &tab_marker);
        }
        else
        {
            juan_set_render_draw_color(renderer, &COLOR_PANE_BG_2);
            SDL_RenderFillRect(renderer, &keyb->tab_geometry[i]);
        }
        SDL_Texture *tex = res_get_keyb_tab_tex(INSTR_THEME_BEFUNGE_CHAR,
                (enum RES_KEYB_TAB_ID) i);
        SDL_Rect rect = { 0, 0, 0, 0 };
        SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
        int text_height = keyb->tab_geometry[i].h * 0.8;
        rect.w *= (float) text_height / rect.h;
        rect.h = text_height;
        rect.x = keyb->tab_geometry[i].x +
                keyb->tab_geometry[i].w / 2 - rect.w / 2;
        rect.y = keyb->tab_geometry[i].y +
                keyb->tab_geometry[i].h / 2 - rect.h / 2;
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw arrows

    if (keyb->active_tab != KEYB_TAB_MISC && keyb->active_tab != KEYB_TAB_RUN)
    {
        SDL_Texture* tex = NULL;
        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        tex = res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_ARROW_UP);

        SDL_RenderFillRect(renderer, &keyb->arrow_buttons[KEYB_ARROW_UP].geometry);
        SDL_RenderCopyEx(renderer, tex, NULL,
                &keyb->arrow_buttons[KEYB_ARROW_UP].geometry, 0, NULL, SDL_FLIP_NONE);

        SDL_RenderFillRect(renderer, &keyb->arrow_buttons[KEYB_ARROW_DOWN].geometry);
        SDL_RenderCopyEx(renderer, tex, NULL,
                &keyb->arrow_buttons[KEYB_ARROW_DOWN].geometry, 180, NULL, SDL_FLIP_NONE);

        SDL_RenderFillRect(renderer, &keyb->arrow_buttons[KEYB_ARROW_LEFT].geometry);
        SDL_RenderCopyEx(renderer, tex, NULL,
                &keyb->arrow_buttons[KEYB_ARROW_LEFT].geometry, 270, NULL, SDL_FLIP_NONE);

        SDL_RenderFillRect(renderer, &keyb->arrow_buttons[KEYB_ARROW_RIGHT].geometry);
        SDL_RenderCopyEx(renderer, tex, NULL,
                &keyb->arrow_buttons[KEYB_ARROW_RIGHT].geometry, 90, NULL, SDL_FLIP_NONE);
    }

    // Draw shift and special buttons

    if (keyb->active_tab != KEYB_TAB_MISC && keyb->active_tab != KEYB_TAB_RUN)
    {
        juan_set_render_draw_color(renderer, &COLOR_BUTTON_3);

        draw_button(renderer, keyb, &keyb->action_buttons[KEYB_ACTION_DELETE],
                RES_KEYB_ICON_DELETE);

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_2);

        draw_button(renderer, keyb, &keyb->action_buttons[KEYB_ACTION_SHIFT],
                RES_KEYB_ICON_SHIFT);

        if (keyb->active_tab != KEYB_TAB_VALUES)
        {
            juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);

            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_SELECT_RECT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_SELECT_RECT),
                    NULL, &keyb->action_buttons[KEYB_ACTION_SELECT_RECT].geometry);
            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_SELECT_PAINT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_SELECT_PAINT),
                    NULL, &keyb->action_buttons[KEYB_ACTION_SELECT_PAINT].geometry);
            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_COPY].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_COPY),
                    NULL, &keyb->action_buttons[KEYB_ACTION_COPY].geometry);
            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_CUT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CUT),
                    NULL, &keyb->action_buttons[KEYB_ACTION_CUT].geometry);
            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_PASTE].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_PASTE),
                    NULL, &keyb->action_buttons[KEYB_ACTION_PASTE].geometry);
            SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_COMMENT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_COMMENT),
                    NULL, &keyb->action_buttons[KEYB_ACTION_COMMENT].geometry);
        }
    }

    // Draw buttons

    juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);

    switch (keyb->active_tab)
    {

        case KEYB_TAB_RUN:
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_START].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_START),
                    NULL, &keyb->run_buttons[KEYB_RUN_START].geometry);
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_STOP].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_STOP),
                    NULL, &keyb->run_buttons[KEYB_RUN_STOP].geometry);
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_STEP].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_STEP),
                    NULL, &keyb->run_buttons[KEYB_RUN_STEP].geometry);
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_RESUME].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_RESUME),
                    NULL, &keyb->run_buttons[KEYB_RUN_RESUME].geometry);
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_FASTER].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_TIME_FASTER),
                    NULL, &keyb->run_buttons[KEYB_RUN_FASTER].geometry);
            SDL_RenderFillRect(renderer, &keyb->run_buttons[KEYB_RUN_SLOWER].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_TIME_SLOWER),
                    NULL, &keyb->run_buttons[KEYB_RUN_SLOWER].geometry);
            break;

        case KEYB_TAB_VALUES:
            for (int i = 0; i < KEYB_VALUES_BUTTONS_TOTAL; i++)
            {
                SDL_RenderFillRect(renderer, &keyb->values_buttons[i].geometry);

                if (keyb->values_buttons[i].id != INSTR_NULL
                        && keyb->values_buttons[i].id != INSTR_SPACE)
                {
                    SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR,
                        keyb->values_buttons[i].id);
                    SDL_RenderCopy(renderer, tex, NULL, &keyb->values_buttons[i].geometry);
                }
            }
            break;

        case KEYB_TAB_MOVIO:
            for (int i = 0; i < KEYB_MOVIO_BUTTONS_TOTAL; i++)
            {
                SDL_RenderFillRect(renderer, &keyb->movio_buttons[i].geometry);

                if (keyb->movio_buttons[i].id != INSTR_NULL
                        && keyb->movio_buttons[i].id != INSTR_SPACE)
                {
                    SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR,
                        keyb->movio_buttons[i].id);
                    SDL_RenderCopy(renderer, tex, NULL, &keyb->movio_buttons[i].geometry);
                }
            }
            break;

        case KEYB_TAB_OPER:
            for (int i = 0; i < KEYB_OPER_BUTTONS_TOTAL; i++)
            {
                SDL_RenderFillRect(renderer, &keyb->oper_buttons[i].geometry);

                if (keyb->oper_buttons[i].id != INSTR_NULL
                        && keyb->oper_buttons[i].id != INSTR_SPACE)
                {
                    SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR,
                        keyb->oper_buttons[i].id);
                    SDL_RenderCopy(renderer, tex, NULL, &keyb->oper_buttons[i].geometry);
                }
            }
            break;

        case KEYB_TAB_MISC:
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_NEW].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_NEW),
                    NULL, &keyb->misc_buttons[KEYB_MISC_NEW].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_LOAD].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_LOAD),
                    NULL, &keyb->misc_buttons[KEYB_MISC_LOAD].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_SAVE].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_SAVE),
                    NULL, &keyb->misc_buttons[KEYB_MISC_SAVE].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_SAVE_AS].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_SAVE_AS),
                    NULL, &keyb->misc_buttons[KEYB_MISC_SAVE_AS].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_T_HELP].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_T_HELP),
                    NULL, &keyb->misc_buttons[KEYB_MISC_T_HELP].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_T_VALUES].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_T_VALUES),
                    NULL, &keyb->misc_buttons[KEYB_MISC_T_VALUES].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_ZOOM_IN].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_ZOOM_IN),
                    NULL, &keyb->misc_buttons[KEYB_MISC_ZOOM_IN].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_ZOOM_OUT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_ZOOM_OUT),
                    NULL, &keyb->misc_buttons[KEYB_MISC_ZOOM_OUT].geometry);
            SDL_RenderFillRect(renderer, &keyb->misc_buttons[KEYB_MISC_QUIT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_QUIT),
                    NULL, &keyb->misc_buttons[KEYB_MISC_QUIT].geometry);
            break;

        case KEYB_TAB_CHAR:
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_GET].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_GET),
                    NULL, &keyb->char_buttons[KEYB_CHAR_GET].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_PUT].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_PUT),
                    NULL, &keyb->char_buttons[KEYB_CHAR_PUT].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_ADD_1].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_ADD_1),
                    NULL, &keyb->char_buttons[KEYB_CHAR_ADD_1].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_SUB_1].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_SUB_1),
                    NULL, &keyb->char_buttons[KEYB_CHAR_SUB_1].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_ADD_10].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_ADD_10),
                    NULL, &keyb->char_buttons[KEYB_CHAR_ADD_10].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_SUB_10].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_SUB_10),
                    NULL, &keyb->char_buttons[KEYB_CHAR_SUB_10].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_ADD_16].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_ADD_16),
                    NULL, &keyb->char_buttons[KEYB_CHAR_ADD_16].geometry);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_SUB_16].geometry);
            SDL_RenderCopy(renderer,
                    res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_CHAR_SUB_16),
                    NULL, &keyb->char_buttons[KEYB_CHAR_SUB_16].geometry);
            break;

        default:
            SDL_Log("Error rendering invalid tab %d\n", keyb->active_tab);
            break;
    }

    // Draw other things

    switch (keyb->active_tab)
    {
        case KEYB_TAB_CHAR:

            // Current character

            juan_set_render_draw_color(renderer, &COLOR_BUTTON_3);
            SDL_RenderFillRect(renderer, &keyb->char_buttons[KEYB_CHAR_DISPLAY].geometry);
            SDL_RenderCopy(renderer,
                    res_get_instr_char_tex(INSTR_THEME_BEFUNGE_CHAR, keyb->curr_char),
                    NULL, &keyb->char_buttons[KEYB_CHAR_DISPLAY].geometry);

            text_draw(renderer, keyb->curr_char_repr);
            break;

        default:
            break;

    }
}

/// Get the ID of the button over the point, used to get the pressed button
/**
 * Takes a SDL_Point and a list of buttons, returns the ID of the button over
 * the point.
 *
 * Returns -1 if there was no button over the point.
 */
static int get_button_on_point(
    SDL_Point *point,
    Button *button_list,
    int button_number
) {
    for (int i = 0; i < button_number; i++)
    {
        if (SDL_PointInRect(point, &button_list[i].geometry))
        {
            return button_list[i].id;
        }
    }
    return -1;
}

KeyboardEvent keyb_handle_input
(
    Keyboard *keyb,
    Input *input
) {
    KeyboardEvent event =
    {
        .type = KEYB_EVENT_NOT_HANDLED,
        .character = 0,
    };

    // Ignore events outside the keyboard

    switch (input->type)
    {
        case (INPUT_CLICK_UP):
            if (!SDL_PointInRect(&input->point, &keyb->geometry)
                    && !SDL_PointInRect(&input->point, &keyb->tabs_geometry))
            {
                return event; // KEYB_EVENT_NOT_HANDLED
            }
            break;
        case (INPUT_CLICK_MOVE):
        case (INPUT_CLICK_MOVE_UP):
            if (!SDL_PointInRect(&input->down_point, &keyb->geometry)
                    && !SDL_PointInRect(&input->down_point, &keyb->tabs_geometry))
            {
                return event; // KEYB_EVENT_NOT_HANDLED
            }
            break;
        default:
            return event; // KEYB_EVENT_NOT_HANDLED
    }

    // Indicate that at least the input was handled
    event.type = KEYB_EVENT_NONE;

    if (input->type == INPUT_CLICK_MOVE)
    {
        if (SDL_PointInRect(&input->down_point, &keyb->tabs_geometry))
        {
            keyb->tabs_offset += input->diff.x;
            if (keyb->tabs_offset < keyb->tabs_min_offset) {
                keyb->tabs_offset = keyb->tabs_min_offset;
            }
            if (keyb->tabs_offset > 0) {
                keyb->tabs_offset = 0;
            }
            update_buttons(keyb);
        }
    }
    else if (input->type == INPUT_CLICK_UP)
    {
        int pressed = -1;

        // Check arrows

        if (keyb->active_tab != KEYB_TAB_MISC)
        {
            pressed = get_button_on_point(&input->point,
                    keyb->arrow_buttons, KEYB_ARROW_BUTTON_ID_TOTAL);

            switch (pressed)
            {
                case KEYB_ARROW_UP:
                    event.type = KEYB_EVENT_MOVE_UP;
                    return event;
                    break;

                case KEYB_ARROW_DOWN:
                    event.type = KEYB_EVENT_MOVE_DOWN;
                    return event;
                    break;

                case KEYB_ARROW_LEFT:
                    event.type = KEYB_EVENT_MOVE_LEFT;
                    return event;
                    break;

                case KEYB_ARROW_RIGHT:
                    event.type = KEYB_EVENT_MOVE_RIGHT;
                    return event;
                    break;
            }
        }

        // Check action buttons

        if (keyb->active_tab != KEYB_TAB_MISC && keyb->active_tab != KEYB_TAB_RUN)
        {
            pressed = get_button_on_point(&input->point,
                    keyb->action_buttons, 2);

            switch (pressed)
            {
                case KEYB_ACTION_DELETE:
                    event.type = KEYB_EVENT_RM_CHAR;
                    return event;
                    break;

                case KEYB_ACTION_SHIFT:
                    // TODO
                    return event;
                    break;
            }

            // Check sometimes hidden action buttons

            if (keyb->active_tab != KEYB_TAB_VALUES)
            {

                pressed = get_button_on_point(&input->point,
                        keyb->action_buttons + 2, KEYB_ACTION_BUTTON_ID_TOTAL);

                switch (pressed)
                {
                    case KEYB_ACTION_COPY:
                        {
                            // TODO
                        }
                        break;
                }
            }
        }

        // Check buttons depending on tabs

        switch (keyb->active_tab)
        {

            case KEYB_TAB_RUN:

                pressed = get_button_on_point(&input->point,
                        keyb->run_buttons, KEYB_RUN_BUTTON_ID_TOTAL);

                switch (pressed)
                {
                    case KEYB_RUN_START:
                        event.type = KEYB_EVENT_START;
                        return event;
                        break;

                    case KEYB_RUN_STOP:
                        event.type = KEYB_EVENT_STOP;
                        return event;
                        break;

                    case KEYB_RUN_STEP:
                        event.type = KEYB_EVENT_STEP;
                        return event;
                        break;

                    case KEYB_RUN_RESUME:
                        event.type = KEYB_EVENT_RESUME;
                        return event;
                        break;

                    case KEYB_RUN_FASTER:
                        event.type = KEYB_EVENT_FASTER;
                        return event;
                        break;

                    case KEYB_RUN_SLOWER:
                        event.type = KEYB_EVENT_SLOWER;
                        return event;
                        break;

                }
                break;

            case KEYB_TAB_VALUES:

                pressed = get_button_on_point(&input->point,
                        keyb->values_buttons, KEYB_VALUES_BUTTONS_TOTAL);

                if (pressed != -1)
                {
                    event.type = KEYB_EVENT_ADD_CHAR;
                    event.character = const_befunge_char(pressed);
                }
                break;

            case KEYB_TAB_MOVIO:

                pressed = get_button_on_point(&input->point,
                        keyb->movio_buttons, KEYB_MOVIO_BUTTONS_TOTAL);

                if (pressed != -1)
                {
                    event.type = KEYB_EVENT_ADD_CHAR;
                    event.character = const_befunge_char(pressed);
                }
                break;

            case KEYB_TAB_OPER:

                pressed = get_button_on_point(&input->point,
                        keyb->oper_buttons, KEYB_OPER_BUTTONS_TOTAL);

                if (pressed != -1)
                {
                    event.type = KEYB_EVENT_ADD_CHAR;
                    event.character = const_befunge_char(pressed);
                }
                break;

            case KEYB_TAB_MISC:

                pressed = get_button_on_point(&input->point,
                        keyb->misc_buttons, KEYB_MISC_BUTTON_ID_TOTAL);

                switch (pressed)
                {
                    case KEYB_MISC_NEW:
                        event.type = KEYB_EVENT_NEW;
                        return event;
                        break;

                    case KEYB_MISC_LOAD:
                        event.type = KEYB_EVENT_LOAD;
                        return event;
                        break;

                    case KEYB_MISC_SAVE:
                        event.type = KEYB_EVENT_SAVE;
                        return event;
                        break;

                    case KEYB_MISC_SAVE_AS:
                        event.type = KEYB_EVENT_SAVE_AS;
                        return event;
                        break;

                    case KEYB_MISC_ZOOM_IN:
                        event.type = KEYB_EVENT_ZOOM_IN;
                        return event;
                        break;

                    case KEYB_MISC_ZOOM_OUT:
                        event.type = KEYB_EVENT_ZOOM_OUT;
                        return event;
                        break;
                }
                break;

            case KEYB_TAB_CHAR:
                pressed = get_button_on_point(&input->point,
                        keyb->char_buttons, KEYB_CHAR_BUTTON_ID_TOTAL);

                int curr_char = (int)keyb->curr_char;
                switch (pressed)
                {
                    case KEYB_CHAR_ADD_1:  curr_char += 1;  break;
                    case KEYB_CHAR_ADD_10: curr_char += 10; break;
                    case KEYB_CHAR_ADD_16: curr_char += 16; break;
                    case KEYB_CHAR_SUB_1:  curr_char -= 1;  break;
                    case KEYB_CHAR_SUB_10: curr_char -= 10; break;
                    case KEYB_CHAR_SUB_16: curr_char -= 16; break;
                    case KEYB_CHAR_PUT:
                        event.type = KEYB_EVENT_ADD_CHAR;
                        event.character = keyb->curr_char;
                        break;
                }
                keyb->curr_char = (char)juan_min(juan_max(curr_char, -128), 127);

                update_curr_char_repr_str(keyb);

                break;

            default:
                SDL_Log("Error rendering invalid tab %d\n", keyb->active_tab);
                break;
        }

        // Check tabs

        for (enum KEYB_TAB_ID i = 0; i < KEYB_TAB_ID_TOTAL; i++)
        {
            if (SDL_PointInRect(&input->point, &keyb->tab_geometry[i]))
            {
                keyb->active_tab = i;
            }
        }
    }
    return event;
}
