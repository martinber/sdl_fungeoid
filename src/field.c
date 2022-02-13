#include "field.h"

Field *field_create(SDL_Point window_size, Intrpr *intrpr)
{
    Field *field = (Field*) malloc(sizeof(Field));
    if (field == NULL)
    {
        SDL_Log("Failed to malloc Field\n");
        return NULL;
    }

    field->_intrpr = intrpr; // A reference, owned by Game
    field->_drag = drag_create();
    field->_zoom_level = 3;
    if (field->_drag == NULL)
    {
        drag_free(field->_drag);
        field->_drag = NULL;

        free(field);
        field = NULL;

        SDL_Log("Error creating field->_drag");
        return NULL;
    }

    // Sets rest of variables
    field_update_geometry(field, window_size);

    return field;
}

void field_free(Field *field)
{
    if (field != NULL)
    {
        drag_free(field->_drag);
        field->_drag = NULL;
    }
    free(field);
    field = NULL;
}

void field_update_geometry(Field *field, SDL_Point window_size)
{
    field->_window_size = window_size;
    float multiplier = (float)field->_zoom_level / 50;
    field->_cell_size = juan_min(
            (int)round(window_size.x * multiplier),
            (int)round(window_size.y * multiplier)
        );
    drag_set_snapping(field->_drag, (float) field->_cell_size, (float) field->_cell_size);
}

void field_update(Field *field, Uint32 time_abs_ms)
{
    drag_update(field->_drag, time_abs_ms);
}

void field_handle_input(Field *field, Input *input)
{
    // Offset because of scrolling
    int x_offset = (int) field->_drag->x;
    int y_offset = (int) field->_drag->y;

    Canvas *canvas = intrpr_get_canvas(field->_intrpr);
    SDL_Point ip = intrpr_get_ip(field->_intrpr);

    if (intrpr_get_state(field->_intrpr) == INTRPR_PAUSED)
    {
        if (input->type == INPUT_CLICK_UP)
        {
            intrpr_move_ip(field->_intrpr,
                    (input->point.x - x_offset) / field->_cell_size,
                    (input->point.y - y_offset) / field->_cell_size
                );
        }
        else if (input->type == INPUT_CLICK_MOVE)
        {
            drag_move(field->_drag, input->point, input->timestamp);
        }
        else if (input->type == INPUT_CLICK_MOVE_UP)
        {
            drag_up(field->_drag);
        }
        else if (input->type == INPUT_TEXT)
        {
            // input->text should be the text given by SDL_TextInputEvent,
            // most of the time contains only a character that represents the
            // letter
            if (strlen(input->text) == 1)
            {
                canvas_set_char(canvas, ip.x, ip.y, input->text[0]);
            }
        }
        else if (input->type == INPUT_KEY_DOWN)
        {
            switch (input->key)
            {
                case SDLK_UP:
                    intrpr_move_ip_rel(field->_intrpr, 0, -1);
                    break;
                case SDLK_DOWN:
                    intrpr_move_ip_rel(field->_intrpr, 0, 1);
                    break;
                case SDLK_LEFT:
                    intrpr_move_ip_rel(field->_intrpr, -1, 0);
                    break;
                case SDLK_RIGHT:
                    intrpr_move_ip_rel(field->_intrpr, 1, 0);
                    break;
                case SDLK_BACKSPACE:
                    canvas_set_char(canvas, ip.x, ip.y, ' ');
                    break;

                default:
                    break;
            }
        }
    }
}

void field_handle_keyb(Field *field, KeyboardEvent *event)
{
    switch (event->type)
    {
        case KEYB_EVENT_FASTER:
            intrpr_faster(field->_intrpr);
            break;
        case KEYB_EVENT_SLOWER:
            intrpr_slower(field->_intrpr);
            break;
        case KEYB_EVENT_ZOOM_IN:
            field->_zoom_level = juan_min(FIELD_ZOOM_LEVEL_MAX, field->_zoom_level + 1);
            field_update_geometry(field, field->_window_size);
            break;
        case KEYB_EVENT_ZOOM_OUT:
            field->_zoom_level = juan_max(1, field->_zoom_level - 1);
            field_update_geometry(field, field->_window_size);
            break;
        default:
            break;
    }
    if (intrpr_get_state(field->_intrpr) == INTRPR_PAUSED)
    {
        Canvas *canvas = intrpr_get_canvas(field->_intrpr);
        SDL_Point ip = intrpr_get_ip(field->_intrpr);

        switch (event->type)
        {
            case KEYB_EVENT_ADD_INSTR:
                canvas_set_char(canvas, ip.x, ip.y, const_befunge_char(event->instr_id));
                break;
            case KEYB_EVENT_RM_INSTR:
                canvas_set_char(canvas, ip.x, ip.y, ' ');
                break;

            case KEYB_EVENT_MOVE_UP:
                intrpr_move_ip_rel(field->_intrpr, 0, -1);
                break;
            case KEYB_EVENT_MOVE_DOWN:
                intrpr_move_ip_rel(field->_intrpr, 0, 1);
                break;
            case KEYB_EVENT_MOVE_LEFT:
                intrpr_move_ip_rel(field->_intrpr, -1, 0);
                break;
            case KEYB_EVENT_MOVE_RIGHT:
                intrpr_move_ip_rel(field->_intrpr, 1, 0);
                break;

            case KEYB_EVENT_START:
                intrpr_reset(field->_intrpr);
                intrpr_resume(field->_intrpr);
                break;

            case KEYB_EVENT_RESUME:
                intrpr_resume(field->_intrpr);
                break;

            case KEYB_EVENT_STEP:
                intrpr_step(field->_intrpr);
                break;

            default:
                break;
        }
    }
    else if (intrpr_get_state(field->_intrpr) == INTRPR_RUNNING)
    {
        switch (event->type)
        {
            case KEYB_EVENT_STOP:
                intrpr_pause(field->_intrpr);
                break;

            default:
                break;
        }
    }
}

void field_draw(SDL_Renderer *renderer, Field *field)
{
    int cell_size = field->_cell_size;
    Canvas *canvas = intrpr_get_canvas(field->_intrpr);

    // Total size of canvas
    int width = canvas_get_width(canvas);
    int height = canvas_get_height(canvas);

    // Offset because of scrolling
    int x_offset = (int) field->_drag->x;
    int y_offset = (int) field->_drag->y;

    // TODO: draw only things on screen

    // Draw lines
    juan_set_render_draw_color(renderer, &COLOR_LINES);
    int line_width = cell_size / 20;

    for (int x = 0; x < width + 1; x++)
    {
        juan_draw_v_line_cap(renderer,
                x * cell_size + x_offset, 0, height * cell_size, line_width);
    }
    for (int y = 0; y < height + 1; y++)
    {
        juan_draw_h_line_cap(renderer,
                0, y * cell_size + y_offset, width * cell_size, line_width);
    }

    // Draw instructions
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            char _c = canvas_get_char(canvas, x, y);
            char c = ' ';

            if (_c < 0) {
                SDL_Log("Error reading canvas char when drawing");
            } else {
                c = (char) _c;
            }

            SDL_Texture* tex = res_get_instr_char_tex(INSTR_THEME_BEFUNGE_CHAR, c);
            SDL_Rect r = {
                x * cell_size + x_offset,
                y * cell_size + y_offset,
                cell_size,
                cell_size
            };
            SDL_RenderCopy(renderer, tex, NULL, &r);

            continue; // TODO: Optimize number drawing

            // Draw ascii value
            char str_buf[4];
            snprintf(str_buf, 4, "%d", c);
            str_buf[3] = '\0';
            tex = juan_text_texture(
                renderer,
                res_get_font(RES_FONT_STACK),
                str_buf,
                COLOR_WHITE
            );
            if (tex == NULL)
            {
                SDL_Log("Error rendering text for canvas value: %d", c);
                return;
            }
            SDL_Rect rect;
            SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
            rect.x = x * cell_size + x_offset;
            rect.y = y * cell_size + y_offset;
            double factor = (cell_size / 2) / (float) rect.h;
            rect.h *= factor;
            rect.w *= factor;
            SDL_RenderCopy(renderer, tex, NULL, &rect);
            SDL_DestroyTexture(tex);
        }
    }

    // Draw IP

    SDL_Point ip = intrpr_get_ip(field->_intrpr);
    SDL_Point ip_speed = intrpr_get_ip_speed(field->_intrpr);
    int ip_width = cell_size / 8;

    juan_set_render_draw_color(renderer, &COLOR_SELECT_1);
    juan_draw_h_line_cap(renderer,
            ip.x * cell_size + x_offset,
            ip.y * cell_size + y_offset,
            cell_size, ip_width);
    juan_draw_h_line_cap(renderer,
            ip.x * cell_size + x_offset,
            (ip.y + 1) * cell_size + y_offset,
            cell_size, ip_width);
    juan_draw_v_line_cap(renderer,
            ip.x * cell_size + x_offset,
            ip.y * cell_size + y_offset,
            cell_size, ip_width);
    juan_draw_v_line_cap(renderer,
            (ip.x + 1) * cell_size + x_offset,
            ip.y * cell_size + y_offset,
            cell_size, ip_width);
    if (ip_speed.x == 1)
    {
        juan_draw_v_line_cap(renderer,
                ip.x * cell_size + x_offset + ip_width,
                ip.y * cell_size + y_offset,
                cell_size, ip_width);
    }
    else if (ip_speed.x == -1)
    {
        juan_draw_v_line_cap(renderer,
                (ip.x + 1) * cell_size + x_offset - ip_width,
                ip.y * cell_size + y_offset,
                cell_size, ip_width);
    }
    else if (ip_speed.y == 1)
    {
        juan_draw_h_line_cap(renderer,
                ip.x * cell_size + x_offset,
                ip.y * cell_size + y_offset + ip_width,
                cell_size, ip_width);
    }
    else if (ip_speed.y == -1)
    {
        juan_draw_h_line_cap(renderer,
                ip.x * cell_size + x_offset,
                (ip.y + 1) * cell_size + y_offset - ip_width,
                cell_size, ip_width);
    }
}
