#include "keyb.h"

#include "res.h"

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

static const int TAB_NUMBER = 5;

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
    keyb->active_tab = 0;
    keyb->shift_state = KEYB_SHIFT_NONE;

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
    for (int i = 0; i < KEYB_ARROW_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->arrow_buttons[i], BUTTON_KEYB_ARROW, i);
    }
    for (int i = 0; i < KEYB_ACTION_BUTTON_ID_TOTAL; i++)
    {
        button_init(&keyb->action_buttons[i], BUTTON_KEYB_ACTION, i);
    }

    keyb_update_geometry(keyb, window_size);
    return keyb;
}

void keyb_free(Keyboard *keyb)
{
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


void keyb_update_geometry(Keyboard *keyb, SDL_Point window_size) {

    int button_size = juan_min(window_size.x / 9, window_size.y / 12);
    int tab_height = button_size * 0.7;
    int tab_width = button_size * 1.7;

    // Set keyboard size
    keyb->geometry.w = window_size.x;
    keyb->geometry.h = button_size * 5.2;
    keyb->geometry.x = 0;
    keyb->geometry.y = window_size.y - keyb->geometry.h;

    int margin = button_size / 2;
    int spacing = button_size / 8;
    int keyb_x = keyb->geometry.x;
    int keyb_y = keyb->geometry.y;
    int keyb_w = keyb->geometry.w;
    int keyb_h = keyb->geometry.h;

    // Tabs
    for (enum KEYB_TAB_ID i = 0; i < KEYB_TAB_ID_TOTAL; i++)
    {
        int origin_x = keyb_x;
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

    // Buttons
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
    for (int i = 0; i < KEYB_VALUES_BUTTONS_TOTAL; i++)
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

    // Draw shift and special buttons

    juan_set_render_draw_color(renderer, &COLOR_BUTTON_3);

    SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_DELETE].geometry);
    SDL_RenderCopy(renderer,
            res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_DELETE),
            NULL, &keyb->action_buttons[KEYB_ACTION_DELETE].geometry);

    juan_set_render_draw_color(renderer, &COLOR_BUTTON_2);

    SDL_RenderFillRect(renderer, &keyb->action_buttons[KEYB_ACTION_SHIFT].geometry);
    SDL_RenderCopy(renderer,
            res_get_keyb_icon_tex(INSTR_THEME_BEFUNGE_CHAR, RES_KEYB_ICON_SHIFT),
            NULL, &keyb->action_buttons[KEYB_ACTION_SHIFT].geometry);

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

    // Draw buttons

    juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);

    switch (keyb->active_tab)
    {

        case KEYB_TAB_RUN:
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

        case KEYB_TAB_CHAR:
            break;

        default:
            SDL_Log("Error rendering invalid tab %d\n", keyb->active_tab);
            break;
    }
}

KeyboardEvent keyb_handle_input
(
    Keyboard *keyb,
    Input *input
) {
    KeyboardEvent event =
    {
        .type = KEYB_EVENT_NOT_HANDLED,
        .instr_id = INSTR_NULL,
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

    // Indicate that the input was handled
    event.type = KEYB_EVENT_NONE;

    if (input->type == INPUT_CLICK_UP)
    {
        // Check arrows

        if (SDL_PointInRect(&input->point, &keyb->arrow_buttons[KEYB_ARROW_UP].geometry))
        {
            event.type = KEYB_EVENT_MOVE_UP;
        }
        if (SDL_PointInRect(&input->point, &keyb->arrow_buttons[KEYB_ARROW_DOWN].geometry))
        {
            event.type = KEYB_EVENT_MOVE_DOWN;
        }
        if (SDL_PointInRect(&input->point, &keyb->arrow_buttons[KEYB_ARROW_LEFT].geometry))
        {
            event.type = KEYB_EVENT_MOVE_LEFT;
        }
        if (SDL_PointInRect(&input->point, &keyb->arrow_buttons[KEYB_ARROW_RIGHT].geometry))
        {
            event.type = KEYB_EVENT_MOVE_RIGHT;
        }

        // Check shift buttons

        else if (SDL_PointInRect(&input->point, &keyb->action_buttons[KEYB_ACTION_DELETE].geometry))
        {
            event.type = KEYB_EVENT_START;
        }
        else if (SDL_PointInRect(&input->point, &keyb->action_buttons[KEYB_ACTION_SHIFT].geometry))
        {
            event.type = KEYB_EVENT_STOP;
        }
        if (keyb->active_tab != KEYB_TAB_VALUES)
        {


        }

        // Check buttons
        switch (keyb->active_tab)
        {

            case KEYB_TAB_RUN:
                break;

            case KEYB_TAB_VALUES:
                for (int i = 0; i < KEYB_VALUES_BUTTONS_TOTAL; i++)
                {
                    if (SDL_PointInRect(&input->point, &keyb->values_buttons[i].geometry))
                    {
                        if (
                            keyb->values_buttons[i].id != INSTR_NULL
                            && keyb->values_buttons[i].id != INSTR_SPACE
                        ) {
                            event.type = KEYB_EVENT_ADD_INSTR;
                            event.instr_id = keyb->values_buttons[i].id;
                        }
                        else
                        {
                            event.type = KEYB_EVENT_RM_INSTR;
                            event.instr_id = keyb->values_buttons[i].id;
                        }
                        break;
                    }
                }
                break;

            case KEYB_TAB_MOVIO:
                for (int i = 0; i < KEYB_MOVIO_BUTTONS_TOTAL; i++)
                {
                    if (SDL_PointInRect(&input->point, &keyb->movio_buttons[i].geometry))
                    {
                        if (
                            keyb->movio_buttons[i].id != INSTR_NULL
                            && keyb->movio_buttons[i].id != INSTR_SPACE
                        ) {
                            event.type = KEYB_EVENT_ADD_INSTR;
                            event.instr_id = keyb->movio_buttons[i].id;
                        }
                        else
                        {
                            event.type = KEYB_EVENT_RM_INSTR;
                            event.instr_id = keyb->movio_buttons[i].id;
                        }
                        break;
                    }
                }
                break;

            case KEYB_TAB_OPER:
                for (int i = 0; i < KEYB_OPER_BUTTONS_TOTAL; i++)
                {
                    if (SDL_PointInRect(&input->point, &keyb->oper_buttons[i].geometry))
                    {
                        if (
                            keyb->oper_buttons[i].id != INSTR_NULL
                            && keyb->oper_buttons[i].id != INSTR_SPACE
                        ) {
                            event.type = KEYB_EVENT_ADD_INSTR;
                            event.instr_id = keyb->oper_buttons[i].id;
                        }
                        else
                        {
                            event.type = KEYB_EVENT_RM_INSTR;
                            event.instr_id = keyb->oper_buttons[i].id;
                        }
                        break;
                    }
                }
                break;

            case KEYB_TAB_CHAR:
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
    else if (input->type == INPUT_CLICK_MOVE)
    {
        // TODO: Move tabs?

    }
    return event;
}
